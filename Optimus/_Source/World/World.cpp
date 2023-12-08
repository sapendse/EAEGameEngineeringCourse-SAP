#include <sstream>
#include "World.h"
#include "..\Physics\Physics.h"
#include "..\Renderer\Renderer.h"
#include "..\Collision\Collision.h"
#include "..\Timer\TimeSystem.h"
#include "..\Utilities\Utilities.h"
#include "..\Utilities\DebugLines.h"
#include "..\DebugConsole\DebugConsole.h"
#include "..\Sounds\SoundManger.h"
#include "..\Networking\NetworkManager.h"

Optimus::Physics* Optimus::Physics::m_instance = NULL;
Optimus::World* Optimus::World::m_instance = NULL;
Optimus::TimeSystem* Optimus::TimeSystem::m_instance = NULL;
Optimus::Renderer* Optimus::Renderer::m_instance = NULL;
Optimus::Collision* Optimus::Collision::m_instance = NULL;
Optimus::Messaging* Optimus::Messaging::m_instance = NULL;
Optimus::F32 Optimus::TimeSystem::deltaTime = 0;
Optimus::F32 Optimus::Clock::s_cyclesPerSecond = 0;
Optimus::U64 Optimus::Actor::ID = 0;
Optimus::U64 Optimus::GameObject::ID =0;
Optimus::MemoryPool* Optimus::Vector3::p_myMemPool = NULL;
Optimus::cTPSCamera* Optimus::cTPSCamera::m_instance = NULL;
//Optimus::cFPSCamera* Optimus::cFPSCamera::m_instance = NULL;
Optimus::WayPointGraph* Optimus::WayPointGraph::m_instance = NULL;
Optimus::SoundManager* Optimus::SoundManager::m_instance = NULL;
Optimus::NetworkManager* Optimus::NetworkManager::m_instance= NULL;

bool Optimus::World::addToWorld( SharedPointer<Actor> i_newObject )
{
	//Add the object to the list.
	m_listOfWorldObjects.push_back( i_newObject );
	if( i_newObject->getPlayer() )
	{
		cMesh* playerMesh;
		for( unsigned int i = 0; i < cSceneHandler::getInstance()->m_opaqueEnts.size(); i++ )
		{
			if( cSceneHandler::getInstance()->m_opaqueEnts[i]->m_player == true )
			{
				playerMesh = cSceneHandler::getInstance()->m_opaqueEnts[i];
			}
		}
		i_newObject->m_gameObject->setMesh( playerMesh );
	}
	if( i_newObject->m_physics == true )
	{
		Physics::getInstance()->addActor( i_newObject->m_gameObject );
	}
	if( i_newObject->m_collision == true )
	{
		Collision::getInstance()->addActor( i_newObject->m_gameObject );
	}
	if( i_newObject->m_rendering == true )
	{
		Renderer::getInstance()->addActor( i_newObject->m_gameObject );
	}
	return true;
}

void Optimus::World::clearList()
{
	const unsigned int count = m_listOfWorldObjects.size();

	for( unsigned int i=0; i<count; i++ )
	{
		if( m_listOfWorldObjects[i] )
			m_listOfWorldObjects[i].~SharedPointer();
	}
	m_listOfWorldObjects.resize( 0 );
}

bool Optimus::World::Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HINSTANCE i_thisInstanceOfTheApplication, int i_initialWindowDisplayState, std::string i_sceneName )
{
	clearList();
	if( TimeSystem::getInstance()->Initialize() == false )
		return false;
	NetworkManager::getInstance()->Initialize();
	if( Physics::getInstance()->Initialize() == false )
		return false;	
	if( Collision::getInstance()->Initialize() == false )
		return false;
	float ticks = 0.0f;
	#if defined _PROFILER
	Profile* profile = OPT_NEW Profile();
	profile->start();
#endif	
	if( Renderer::getInstance()->Initialize( i_gameName, i_width, i_height, i_thisInstanceOfTheApplication, i_initialWindowDisplayState, i_sceneName ) == false )
		return false;
	#if defined _PROFILER
	ticks = profile->end();
#endif	
	DebugConsole::getInstance()->AddTextField( "FPS:", m_fpsString.c_str() );

	return true;
}

bool Optimus::World::Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HWND i_window, int i_initialWindowDisplayState, std::string i_sceneName )
{
	clearList();
	if( TimeSystem::getInstance()->Initialize() == false )
		return false;
	NetworkManager::getInstance()->Initialize();
	if( Physics::getInstance()->Initialize() == false )
		return false;	
	if( Collision::getInstance()->Initialize() == false )
		return false;
	float ticks = 0.0f;
	#if defined _PROFILER
	Profile* profile = OPT_NEW Profile();
	profile->start();
#endif	
	if( Renderer::getInstance()->Initialize( i_gameName, i_width, i_height, i_window, i_initialWindowDisplayState, i_sceneName ) == false )
		return false;
	#if defined _PROFILER
	ticks = profile->end();
#endif	
	DebugConsole::getInstance()->AddTextField( "FPS:", m_fpsString.c_str() );

	return true;
}

bool Optimus::World::removeFromWorld( SharedPointer<Actor> i_actor )
{
	unsigned int count = m_listOfWorldObjects.size();
	unsigned int to_be_deleted;
	if( count != 0 )
	{
		for( unsigned int i=0 ; i< count ; i++ )
		{
			if( m_listOfWorldObjects[i]->getID() == i_actor->getID() )
			{
				to_be_deleted = i;
				m_listOfWorldObjects.erase( m_listOfWorldObjects.begin() + to_be_deleted );
				break;
			}
		}
	}
	return true;
}


bool Optimus::World::Update( F32 deltaTime )
{
	NetworkManager::getInstance()->Update();

	DebugConsole::getInstance()->InputHandler();
	
	if( m_listOfWorldObjects.size() != 0 )
	{
		unsigned int count = m_listOfWorldObjects.size();
		for( unsigned int i =0; i< count; i++ )
		{
			if( m_listOfWorldObjects[i] )
			{
				m_listOfWorldObjects[i]->Update();
			}
		}
	}
	
	m_fps = 1 / TimeSystem::getInstance()->getDeltaTime() ;
	std::ostringstream buff;
	buff << m_fps;
	m_fpsString = buff.str();

	if( Physics::getInstance()->Update( TimeSystem::getInstance()->getDeltaTime() ) == false )
		return false;

	if( Collision::getInstance()->Update( TimeSystem::getInstance()->getDeltaTime() ) == false )
		return false;

	SoundManager::getInstance()->Update();

	if( Renderer::getInstance()->Update( TimeSystem::getInstance()->getDeltaTime() ) == false )
		return false;

	if( TimeSystem::getInstance()->Update() == false )
		return false;
	return true;
}

bool Optimus::World::Shutdown()
{
	m_listOfWorldObjects.~vector();	
	delete m_instance;
	NetworkManager::getInstance()->Shutdown();
	Optimus::DebugConsole::getInstance()->Shutdown();
	if( Physics::getInstance()->Shutdown() == false )
		return false;
	if( Collision::getInstance()->Shutdown() == false )
		return false;
	if( TimeSystem::getInstance()->ShutDown() == false )
		return false;
	if( Renderer::getInstance()->Shutdown() == false )
		return false;
	SoundManager::getInstance()->Shutdown();

	delete Vector3::p_myMemPool;
	Vector3::p_myMemPool = NULL;
	//clearList();		
	return true;
}