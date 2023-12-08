#include "Game.h"
#include "PlayerController.h"
#include "FlagController.h"
#include "FlagMessageHandler.h"
#include "GameEndStateTrigger.h"
#include "AIController.h"
#include <sstream>
#include <string>
#include "..\Optimus\_Source\Sounds\SoundManger.h"
#include "AI_NetworkController.h"

Optimus::Actor*			player;
Optimus::Actor*			AI;
Optimus::Actor*			playerFlag;
Optimus::Actor*			enemyFlag;
Optimus::Actor*			pWinArea;
Optimus::Actor*			eWinArea;
PlayerController*		pCon;
PlayerCollider*			pColl;
AIController*			AICon;
AICollider*				AIColl;
AI_NetworkController*	AINetCon;
FlagCollider*			pPFlagColl;
FlagController*			pPFlagCon;
FlagCollider*			pEFlagColl;
FlagController*			pEFlagCon;
FlagMessageHandler*		pMessageHandler;
GameEndStateTrigger*	pPlayerGameEndTrigger;
GameEndStateTrigger*	pEnemyGameEndTrigger;


bool Game::Initialize()
{
	srand( 1 );
	pCon = OPT_NEW PlayerController();
	pColl = OPT_NEW PlayerCollider();
	AICon = OPT_NEW AIController( pCon );
	AIColl = OPT_NEW AICollider();
	AINetCon = OPT_NEW AI_NetworkController( pCon );
	pPFlagColl = OPT_NEW FlagCollider();
	pPFlagCon = OPT_NEW FlagController();
	pEFlagColl = OPT_NEW FlagCollider();
	pEFlagCon = OPT_NEW FlagController();
	pMessageHandler = OPT_NEW FlagMessageHandler( pPFlagCon, pCon, pEFlagCon, AICon );
	
	Optimus::Messaging::getInstance()->AddMesage("PFlagCaptured");
	Optimus::Messaging::getInstance()->AddMesage("EFlagCaptured");
	Optimus::Messaging::getInstance()->AddMesage("PlayerWon");
	Optimus::Messaging::getInstance()->AddMesage("EnemyWon");
	std::vector<Optimus::cMesh*> flagMeshes;
	std::vector<Optimus::Vector3> _pos, _size;
	parseDynamicFile( "../WorldEditor/TriggersBin.bin", flagMeshes, _pos, _size );

	Optimus::Vector3 tempMin, tempMax, playerStartPos, AIStartPos;
	
	playerStartPos = ( flagMeshes[1]->m_maxBound + flagMeshes[1]->m_minBound ) / 2.0f;
	AIStartPos = ( flagMeshes[0]->m_maxBound + flagMeshes[0]->m_minBound ) / 2.0f;
	//Player
	player = OPT_NEW  Optimus::Actor( 0.0f, 3.0f, true, false, true, true, "NULL", 0.0f, 0.0f, 0.0f );
	player->setActorController(pCon);
	player->m_gameObject->setCollisionController( pColl );
	player->m_gameObject->setMessageController( pMessageHandler );
	//player->m_gameObject->setPosition( playerStartPos );
	player->m_gameObject->setAcceleration( Optimus::Vector3( 0.0f ) );
	player->m_gameObject->setVelocity( Optimus::Vector3( 0.0f ) );
	player->m_gameObject->SetSpeed( 100.0f );
	player->setPlayer( true );
	player->m_gameObject->setTag( "Player" );
	Optimus::Messaging::getInstance()->Subscribe( "PlayerWon", const_cast<Optimus::MessageCallbackI*>(player->m_gameObject->getMessageController()) );

	//AI
	AI = OPT_NEW  Optimus::Actor( 0.0f, 3.0f, true, false, true, false, "NULL", 0.0f, 0.0f, 0.0f );
	AI->setActorController(AICon);
	AI->m_gameObject->setCollisionController( AIColl );
	AI->m_gameObject->setMesh( Optimus::cSceneHandler::getInstance()->m_opaqueEnts[1] );
	AI->m_gameObject->setMessageController( pMessageHandler );
	AI->m_gameObject->setPosition( Optimus::Vector3( AIStartPos.x(), 70.0f, AIStartPos.z() + 15.0f ) );
	AI->m_gameObject->setAcceleration( Optimus::Vector3( 0.0f ) );
	AI->m_gameObject->setVelocity( Optimus::Vector3( 0.0f ) );
	AI->m_gameObject->SetSpeed( 100.0f );
	AI->m_gameObject->setTag( "AI" );
	Optimus::Messaging::getInstance()->Subscribe( "EnemyWon", const_cast<Optimus::MessageCallbackI*>(AI->m_gameObject->getMessageController()) );

	//Player's Flag
	playerFlag = OPT_NEW Optimus::Actor( 0.0f, 3.0f, false, false, true, false, "NULL", 0.0f, 0.0f, 0.0f );
	playerFlag->setActorController( pPFlagCon );
	playerFlag->m_gameObject->setCollisionController( pPFlagColl );
	playerFlag->m_gameObject->setMesh( flagMeshes[0] );
	playerFlag->m_gameObject->setMessageController( pMessageHandler );
	playerFlag->m_gameObject->setPosition( Optimus::v_Zero );
	playerFlag->m_gameObject->setAcceleration( Optimus::v_Zero );
	playerFlag->m_gameObject->setVelocity( Optimus::v_Zero );
	playerFlag->setPlayer( false );
	playerFlag->m_gameObject->setTag( "PlayerFlag" );
	playerFlag->m_gameObject->setBounds( playerFlag->m_gameObject->getMinBound() * 2.0f, playerFlag->m_gameObject->getMaxBound() * 2.0f );
	Optimus::Messaging::getInstance()->Subscribe( "PFlagCaptured", const_cast<Optimus::MessageCallbackI*>(playerFlag->m_gameObject->getMessageController()) );

	//Enemy's Flag
	enemyFlag = OPT_NEW Optimus::Actor( 0.0f, 3.0f, false, false, true, false, "NULL", 0.0f, 0.0f, 0.0f );
	enemyFlag->setActorController( pEFlagCon );
	enemyFlag->m_gameObject->setMesh( flagMeshes[1] );
	enemyFlag->m_gameObject->setMessageController( pMessageHandler );
	enemyFlag->m_gameObject->setCollisionController( pEFlagColl );
	enemyFlag->m_gameObject->setPosition( Optimus::v_Zero );
	enemyFlag->m_gameObject->setAcceleration( Optimus::v_Zero );
	enemyFlag->m_gameObject->setVelocity( Optimus::v_Zero );
	enemyFlag->setPlayer( false );
	enemyFlag->m_gameObject->setTag( "EnemyFlag" );
	Optimus::Messaging::getInstance()->Subscribe( "EFlagCaptured", const_cast<Optimus::MessageCallbackI*>(playerFlag->m_gameObject->getMessageController()) );

	//Player's Trigger
	tempMin = _pos[1] - _size[1];
	tempMax = _pos[1] + _size[1];
	pPlayerGameEndTrigger = OPT_NEW GameEndStateTrigger();
	pWinArea = OPT_NEW Optimus::Actor( 0.0f,3.0f, false, false, true, false, "NULL", 0.0f, 0.0f, 0.0f );
	pWinArea->m_gameObject->setCollisionController( NULL );
	pWinArea->m_gameObject->setTriggerController( pPlayerGameEndTrigger );
	pWinArea->m_gameObject->setBounds( tempMin, tempMax );
	pWinArea->m_gameObject->setMesh( NULL );
	pWinArea->setPlayer( false );
	pWinArea->m_gameObject->setTag( "PlayerWinArea" );

	//Enemy's Trigger
	tempMin = _pos[0] - _size[0];
	tempMax = _pos[0] + _size[0];
	pEnemyGameEndTrigger = OPT_NEW GameEndStateTrigger();
	eWinArea = OPT_NEW Optimus::Actor( 0.0f,3.0f, false, false, true, false, "NULL", 0.0f, 0.0f, 0.0f );
	eWinArea->m_gameObject->setCollisionController( NULL );
	eWinArea->m_gameObject->setTriggerController( pEnemyGameEndTrigger );
	eWinArea->m_gameObject->setBounds( tempMin, tempMax );
	eWinArea->m_gameObject->setMesh( NULL );
	eWinArea->setPlayer( false );
	eWinArea->m_gameObject->setTag( "EnemyWinArea" );
	AICon->SetDestination( enemyFlag->m_gameObject->getWorldPosition() );
	pMessageHandler->SetEnemyFlagArea( enemyFlag->m_gameObject->getWorldPosition() );
	pMessageHandler->SetEnemyWinArea( eWinArea->m_gameObject->getWorldPosition() );

	//Scores
	std::ostringstream buff;
	buff << pCon->GetScore();
	playerScore = "Player's Score: ";
	playerScore = playerScore + buff.str();
	playerScoreText = Optimus::cSceneHandler::getInstance()->AddText( 10, 5, playerScore.c_str(), 25, 25,125, 75, D3DCOLOR_XRGB( 0, 255, 0 ), true );
	buff.str("");
	buff.clear();
	buff << AICon->GetScore();
	enemyScore = "Enemy's Score: ";
	enemyScore = enemyScore + buff.str();
	enemyScoreText = Optimus::cSceneHandler::getInstance()->AddText( 10, 5, enemyScore.c_str(), 25, 50, 125, 100, D3DCOLOR_XRGB( 255, 0, 0 ), true );
	buff.str("");
	buff.clear();
	buff << pCon->GetStamina();
	playerStamina = "Player Stamina: ";
	playerStamina = playerStamina + buff.str();
	playerStaminaText = Optimus::cSceneHandler::getInstance()->AddText( 10, 5, playerStamina.c_str(), 750, 25, 850, 75, D3DCOLOR_XRGB( 0, 255, 0 ), true );
	buff.str("");
	buff.clear();
	buff << AICon->GetStamina();
	enemyStamina = "Enemy Stamina: ";
	enemyStamina = enemyStamina + buff.str();
	enemyStaminaText = Optimus::cSceneHandler::getInstance()->AddText( 10, 5, enemyStamina.c_str(), 750, 50, 850, 100, D3DCOLOR_XRGB( 255, 0, 0 ), true );
	buff.str("");
	buff.clear();

	//Sounds
	Optimus::SoundManager::getInstance()->Init();
	Optimus::SoundManager::getInstance()->PlayLoop2D( "../Assets/Sounds/I-Giorni.mp3" ); //Background Music

	//Player Sounds
	pCon->m_hardChannel = Optimus::SoundManager::getInstance()->PlayLoop3D( "../Assets/Sounds/running-hard-surface.wav", pCon->getPosition(), pCon->getPosition() );
	pCon->m_softChannel = Optimus::SoundManager::getInstance()->PlayLoop3D( "../Assets/Sounds/running-gravel-or-dry-leaves-loop.wav", pCon->getPosition(), pCon->getPosition() );
	Optimus::SoundManager::getInstance()->PauseChannel( pCon->m_hardChannel, true );
	Optimus::SoundManager::getInstance()->PauseChannel( pCon->m_softChannel, true );
	pCon->m_sprintChannel = Optimus::SoundManager::getInstance()->PlayLoop3D( "../Assets/Sounds/Thief_Run.wav", pCon->getPosition(), pCon->getPosition() );

	//AI Sounds
	AICon->m_hardChannel = Optimus::SoundManager::getInstance()->PlayLoop3D("../Assets/Sounds/running-hard-surface.wav", AICon->GetPosition(), pCon->getPosition() );
	AICon->m_softChannel = Optimus::SoundManager::getInstance()->PlayLoop3D("../Assets/Sounds/running-gravel-or-dry-leaves-loop.wav", AICon->GetPosition(), pCon->getPosition() );
	Optimus::SoundManager::getInstance()->PauseChannel(AICon->m_hardChannel, true);
	Optimus::SoundManager::getInstance()->PauseChannel(AICon->m_softChannel, true);
	AICon->m_sprintChannel = Optimus::SoundManager::getInstance()->PlayLoop3D("../Assets/Sounds/Thief_Run.wav", AICon->GetPosition(), pCon->getPosition() );
	return true;
}

bool Game::parseDynamicFile( const char* i_fileName, std::vector<Optimus::cMesh*>& i_meshVector, std::vector<Optimus::Vector3>& _pos, std::vector<Optimus::Vector3>& _size )
{
	std::vector<int> _meshIndices;
	std::ifstream dynamicFile;
	int _dynamicEntityNumber, _tempID;
	Optimus::Vector3 _tempPos, _tempSize;
	dynamicFile.open( i_fileName, std::ios::binary | std::ios::in );
	if( dynamicFile.peek() == std::ifstream::traits_type::eof() )
	{
		dynamicFile.close();
		return false;
	}
	dynamicFile.read(reinterpret_cast<char*>(&_dynamicEntityNumber), sizeof( int ) );
	if( _dynamicEntityNumber < 0 )
	{
		dynamicFile.close();
		return false;
	}
	for( int i = 0; i < _dynamicEntityNumber; i++ )
	{
		dynamicFile.read(reinterpret_cast<char*>(&_tempPos), sizeof( Optimus::Vector3 ) );
		dynamicFile.read(reinterpret_cast<char*>(&_tempSize), sizeof( Optimus::Vector3 ) );
		_pos.push_back( _tempPos );
		_size.push_back( _tempSize );
	}
	dynamicFile.read(reinterpret_cast<char*>(&_dynamicEntityNumber), sizeof( int ) );
	if( _dynamicEntityNumber < 0 )
	{
		dynamicFile.close();
		return false;
	}
	for( int i = 0; i < _dynamicEntityNumber; i++ )
	{
		dynamicFile.read(reinterpret_cast<char*>(&_tempID), sizeof( int ) );
		_meshIndices.push_back(_tempID);
	}
	if( _pos.size() != _size.size() )
	{
		dynamicFile.close();
		return false;
	}
	dynamicFile.close();
	for( unsigned int i = 0; i < _meshIndices.size(); i++ )
	{
		i_meshVector.push_back( Optimus::cSceneHandler::getInstance()->m_opaqueEnts[_meshIndices[i]] );
	}
	return true;
}

void Game::SyncGames()
{
	Optimus::PlayerInfo toSend;
	toSend.m_hasFlag = pCon->GetHasFlag();
	toSend.m_score = pCon->GetScore();
	toSend.m_stamina = pCon->GetStamina();
	Optimus::NetworkManager::getInstance()->SendPlayerInfo( toSend );

	AINetCon->m_score = Optimus::NetworkManager::getInstance()->m_playerInfo.m_score;
	AINetCon->m_stamina = Optimus::NetworkManager::getInstance()->m_playerInfo.m_stamina;
	AINetCon->m_hasFlag = Optimus::NetworkManager::getInstance()->m_playerInfo.m_hasFlag;
}

bool Game::Update()
{
	if( Optimus::UserInput::IsKeyPressed( '1' ) )
	{
		Optimus::NetworkManager::getInstance()->CreateServer();
	}
	if( Optimus::UserInput::IsKeyPressed( '2' ) )
	{
		Optimus::NetworkManager::getInstance()->ConnectToServer( "192.168.2.3" );
	}

	//Update Stamina Values.
	if( !Optimus::NetworkManager::getInstance()->GetConnected() )
	{
		std::ostringstream buff;
		buff << pCon->GetStamina();
		std::string temp = "Player Stamina: ";
		temp = temp + buff.str();
		playerStaminaText->SetText(temp);
		buff.str("");
		buff.clear();
		buff << AICon->GetStamina();
		temp = "Enemy Stamina: ";
		temp = temp + buff.str();
		enemyStaminaText->SetText(temp);
		buff.str("");
		buff.clear();
		pMessageHandler->SetEnemyFlagArea( enemyFlag->m_gameObject->getWorldPosition() );
		pMessageHandler->SetEnemyWinArea( eWinArea->m_gameObject->getWorldPosition() );
	}
	else
	{
		SyncGames();
		std::ostringstream buff;
		buff << pCon->GetStamina();
		std::string temp = "Player Stamina: ";
		temp = temp + buff.str();
		playerStaminaText->SetText(temp);
		buff.str("");
		buff.clear();
		buff << AINetCon->m_stamina;
		temp = "Enemy Stamina: ";
		temp = temp + buff.str();
		enemyStaminaText->SetText(temp);
		buff.str("");
		if( !m_modeSwitched )
		{
			Optimus::SoundManager::getInstance()->PauseChannel(AICon->m_currentChannel, true);
			Optimus::SoundManager::getInstance()->PauseChannel(AICon->m_sprintChannel, true );
			AI->setActorController( AINetCon );
			AI->m_gameObject->setVelocity( Optimus::v_Zero );
			if( Optimus::NetworkManager::getInstance()->m_isServer )
			{
				pWinArea->m_gameObject->setTag( "EnemyWinArea" );
				eWinArea->m_gameObject->setTag( "PlayerWinArea" );
				enemyFlag->m_gameObject->setTag( "PlayerFlag" );
				playerFlag->m_gameObject->setTag( "EnemyFlag" );
			}
			m_modeSwitched = true;
		}
		if( AINetCon->m_hasFlag )
		{
			playerFlag->m_gameObject->setWorldPosition( AI->m_gameObject->getNextPos() );
		}
		buff.clear();
	}
	return true;
}

bool Game::ShutDown()
{
	//delete player;
	delete pCon;
	delete AICon;
	delete Optimus::Messaging::getInstance();
	delete pEFlagCon;
	delete pPFlagCon;
	delete pMessageHandler;
	if( playerScoreText )
	{
		delete playerScoreText;
		playerScoreText = NULL;
	}
	if( enemyScoreText )
	{
		delete enemyScoreText;
		enemyScoreText = NULL;
	}
	if( playerStaminaText )
	{
		delete playerStaminaText;
		playerStaminaText = NULL;
	}
	if( enemyStaminaText )
	{
		delete enemyStaminaText;
		enemyStaminaText = NULL;
	}
	if( AINetCon )
	{
		delete AINetCon;
		AINetCon = NULL;
	}
	if( m_instance )
	{
		delete m_instance;
		m_instance = NULL;
	}
	return true;
}