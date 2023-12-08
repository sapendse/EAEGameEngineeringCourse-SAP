#include "Renderer.h"
#include "EntryPoint.h"
#include "cRenderer.h"

bool Optimus::Renderer::addActor( SharedPointer<GameObject> i_gameObject )
{
	m_listOfRenderableObjects.push_back( i_gameObject );
	CreateSprite();
	return true;
}

void Optimus::Renderer::clearActorList()
{
	const unsigned int actorCount = m_listOfRenderableObjects.size();
	for( unsigned int i =0; i< actorCount; i++ )
	{
			if( m_listOfRenderableObjects[i] )
				m_listOfRenderableObjects[i].~SharedPointer();
	}
	m_listOfRenderableObjects.resize( 0 );
}

void Optimus::Renderer::CreateSprite()
{
	
}


bool Optimus::Renderer::removeActor( SharedPointer<GameObject> i_gameObject )
{
	const unsigned int actorCount = m_listOfRenderableObjects.size();
	unsigned int tempInd;
	for( unsigned int i=0; i< actorCount; i++ )
	{
		if(  m_listOfRenderableObjects[i]->getID() == i_gameObject->getID() )
		{
			tempInd = i;
			m_listOfRenderableObjects.erase( m_listOfRenderableObjects.begin() + i );
			break;
		}
	}
	return true;
}

bool Optimus::Renderer::setScrRes( u32 i_width, u32 i_height )
{
	m_scrWidth = i_width;
	m_scrHeight = i_height;
	return true;
}

bool Optimus::Renderer::Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HINSTANCE i_thisInstanceOfTheApplication, int i_initialWindowDisplayState, std::string i_sceneName )
{
	clearActorList();
	if( setScrRes( i_width,i_height ) == false )
		return false;
	int exitCode;
	exitCode = cInitialise( i_width, i_height, i_thisInstanceOfTheApplication, i_initialWindowDisplayState, i_sceneName );
	if( exitCode == -1 )
		return false;
	//FBXImporter::getInstance()->InitiateFBX(); 
	return true;
}

bool Optimus::Renderer::Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HWND& i_window, int i_initialWindowDisplayState, std::string i_sceneName )
{
	clearActorList();
	if( setScrRes( i_width,i_height ) == false )
		return false;
	int exitCode;
	exitCode = cInitialise( i_width, i_height, i_window, i_initialWindowDisplayState, i_sceneName );
	if( exitCode == -1 )
		return false;
	//FBXImporter::getInstance()->InitiateFBX(); 
	return true;
}

bool Optimus::Renderer::Update( F32 i_deltaTime )
{
	const unsigned int actorCount = m_listOfRenderableObjects.size();
	
	int exitCode;
	exitCode = cUpdate();
	if( exitCode == -1 )
		return false;
	return true;
}

bool Optimus::Renderer::Shutdown()
{
	//clearActorList();
	cShutDown();
	m_listOfRenderableObjects.~vector();
	delete m_instance;
	return true;
}