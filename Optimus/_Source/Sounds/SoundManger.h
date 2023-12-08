#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <string>
#include <vector>
#include "../Math/Vector3.h"
#include <fmod.h>
#include "fmod.hpp"
#include "fmod_errors.h"
#include "..\Utilities\New.h"

struct ChannelInfo
{
	FMOD::Channel* m_channel;
	Optimus::Vector3 m_listenerPos;
	Optimus::Vector3 m_sourcePos;
};

namespace Optimus
{
	class SoundManager
	{ 
	private:
		FMOD::System*				system;
		FMOD_RESULT					result;
		unsigned int				version;
		int							numDrivers;
		FMOD_SPEAKERMODE			speakerMode;
		FMOD_CAPS					caps;
		char						m_name[256];
		FMOD::Channel*				m_oneShotChannel;

		std::vector<ChannelInfo>	m_listChannels;
		std::vector<FMOD::Channel*>		m_2DListChannels;
		SoundManager() {}

	public:

		static SoundManager* m_instance;
		static SoundManager* getInstance(){ 
			if( m_instance == 0 )
				m_instance = OPT_NEW SoundManager();
			return m_instance;
		}

		void			Init();
		void			PlayOnce( std::string i_soundName );
		FMOD::Channel*	PlayLoop3D( std::string i_soundName, Optimus::Vector3& i_sourcePosition, Optimus::Vector3& i_listenerPosition );
		void			PlayLoop2D( std::string i_soundName );
		void			PauseChannel( FMOD::Channel* i_channel, bool i_value );
		void			PlayOnChannel( FMOD::Channel* i_channel, std::string i_soundName );
		void			ChangeAudioOnChannel( FMOD::Channel* i_channelName, std::string i_soundName );
		void			SetMusicVolume( float i_value );
		void			SetSfxVolume( float i_value );
		void			Update();
		void			Release();
		void			Shutdown();
	};
}

#endif //_SOUND_MANAGER_H_