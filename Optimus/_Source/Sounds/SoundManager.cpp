#include "SoundManger.h"
#include "DebugConsole\DebugConsole.h"

void FMODErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		exit(-1);
	}
}

static float m_musicVolume( 0.2f );
static float m_sfxVolume( 0.4f );

void AdjustSFXVolume( const char* i_menuName, float i_floatValue )
{
	m_sfxVolume = i_floatValue / 255.0f;
}

void AdjustSoundVolume( const char* i_menuName, float i_floatValue )
{
	m_musicVolume = i_floatValue / 255.0f;
}

void Optimus::SoundManager::Init()
{
	result = FMOD::System_Create(&system);
	FMODErrorCheck(result);

	// Get number of sound cards
	result = system->getNumDrivers(&numDrivers);
	FMODErrorCheck(result);

	// No sound cards (disable sound)
	if (numDrivers == 0)
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		FMODErrorCheck(result);
	}

	// At least one sound card
	else
	{
		// Get the capabilities of the default (0) sound card
		result = system->getDriverCaps(0, &caps, 0, &speakerMode);
		FMODErrorCheck(result);

		// Set the speaker mode to match that in Control Panel
		result = system->setSpeakerMode(speakerMode);
		FMODErrorCheck(result);
	}

	// Increase buffer size if user has Acceleration slider set to off
	if (caps & FMOD_CAPS_HARDWARE_EMULATED)
	{
		result = system->setDSPBufferSize(1024, 10);
		FMODErrorCheck(result);
	}

	// Get name of driver
	result = system->getDriverInfo(0, m_name, 256, 0);
	FMODErrorCheck(result);

	// Initialise FMOD
	result = system->init(100, FMOD_INIT_NORMAL, 0);

	DebugConsole::getInstance()->AddSlider( "SFX Volume: ", 0.0f, 1.0f, &AdjustSFXVolume );
	DebugConsole::getInstance()->AddSlider( "Sound Volume: ", 0.0f, 1.0f, &AdjustSoundVolume );
}

void Optimus::SoundManager::PlayOnce( std::string i_soundName )
{
	FMOD::Sound *audio;
	bool isPlaying;
	m_oneShotChannel->isPlaying( &isPlaying );
	if( !isPlaying )
	{
		system->createSound(i_soundName.c_str(), FMOD_DEFAULT, 0, &audio);
		system->playSound(FMOD_CHANNEL_FREE, audio, false, &m_oneShotChannel);
		m_oneShotChannel->setVolume(m_sfxVolume);
	}
}

void Optimus::SoundManager::PlayLoop2D( std::string i_soundName )
{
	FMOD::Sound *audio;
	FMOD::Channel *channel=0;

	system->createSound( i_soundName.c_str(), FMOD_LOOP_NORMAL, 0, &audio );
	system->playSound( FMOD_CHANNEL_FREE, audio, false, &channel );
	channel->setVolume( m_musicVolume ); 
	m_2DListChannels.push_back( channel );
}

void Optimus::SoundManager::SetMusicVolume( float i_value )
{
	m_musicVolume = i_value;

	for( int i=0; i<m_2DListChannels.size(); ++i )
	{
		m_2DListChannels[i]->setVolume( m_musicVolume );
	}
}

void Optimus::SoundManager::SetSfxVolume( float i_value )
{
	m_sfxVolume = i_value;
	for( int i=0; i<m_listChannels.size(); ++i )
	{
		m_listChannels[i].m_channel->setVolume( m_sfxVolume );
	}
}

void Optimus::SoundManager::ChangeAudioOnChannel( FMOD::Channel* i_channelName, std::string i_soundName )
{
	FMOD::Sound *audio;
	system->createSound( i_soundName.c_str(), FMOD_LOOP_NORMAL|FMOD_3D, 0, &audio );
	system->playSound( FMOD_CHANNEL_REUSE, audio, false, &i_channelName );
	i_channelName->setVolume( m_musicVolume );
}

FMOD::Channel* Optimus::SoundManager::PlayLoop3D( std::string i_soundName, Optimus::Vector3& i_sourcePosition, Optimus::Vector3& i_listenerPosition )
{
	FMOD::Sound *audio;
	FMOD::Channel *channel=0;

	system->createSound( i_soundName.c_str(), FMOD_LOOP_NORMAL|FMOD_3D, 0, &audio );
	system->playSound( FMOD_CHANNEL_FREE, audio, false, &channel );

	channel->setVolume( m_sfxVolume ); 

	//need this for sound fall off
	channel->set3DMinMaxDistance( 50,10000 );

	//update position
	//position of listener needed for spatial & reverb effects
	//velocity of listener needed for dopper effects
	FMOD_VECTOR  listenerpos  = { i_listenerPosition.x(), i_listenerPosition.y(), i_listenerPosition.z() };
	system->set3DListenerAttributes( 0, &listenerpos, 0, 0, 0 );

	ChannelInfo newChannel;
	newChannel.m_channel= channel;
	newChannel.m_listenerPos= i_listenerPosition;
	newChannel.m_sourcePos= i_sourcePosition;
	m_listChannels.push_back( newChannel );

	return channel;
}

void Optimus::SoundManager::Update()
{
	system->update();

	SetMusicVolume(m_musicVolume);
	SetSfxVolume(m_sfxVolume);

	for(int i=0; i< m_listChannels.size(); ++i)
	{
		if(m_listChannels[i].m_channel)
		{
			//Update listener position
			FMOD_VECTOR  listenerpos  = { m_listChannels[i].m_listenerPos.x(), m_listChannels[i].m_listenerPos.y(), m_listChannels[i].m_listenerPos.z() };
			system->set3DListenerAttributes(0, &listenerpos, 0, 0, 0);

			//Update source position
			FMOD_VECTOR  sourcePos  = { m_listChannels[i].m_sourcePos.x(), m_listChannels[i].m_sourcePos.y(), m_listChannels[i].m_sourcePos.z() };

			//Stationary Source
			m_listChannels[i].m_channel->set3DAttributes(&sourcePos,0);
		}
	}
}

void Optimus::SoundManager::PauseChannel( FMOD::Channel* i_channel, bool i_value )
{
	i_channel->setPaused( i_value );
}

void Optimus::SoundManager::PlayOnChannel( FMOD::Channel* i_channel, std::string i_soundName )
{
	FMOD::Sound *audio;

	system->createSound( i_soundName.c_str(), FMOD_LOOP_NORMAL|FMOD_3D, 0, &audio );
	system->playSound( FMOD_CHANNEL_FREE, audio, false, &i_channel );
}

void Optimus::SoundManager::Release()
{
	system->release();
}

void Optimus::SoundManager::Shutdown()
{
	if( m_instance )
	{
		delete m_instance;
		m_instance = NULL;
	}
}