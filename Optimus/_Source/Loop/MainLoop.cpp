#include "Optimus.h"

bool Optimus::Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HINSTANCE i_thisInstanceOfTheApplication, int i_initialWindowDisplayState, std::string i_sceneName )
{
	if( World::getInstance()->Initialize( i_gameName, i_width, i_height, i_thisInstanceOfTheApplication, i_initialWindowDisplayState, i_sceneName ) == false )
		return false;
	
	return true;
}

bool Optimus::Update()
{
	if( World::getInstance()->Update( TimeSystem::getInstance()->getDeltaTime() ) == false )
		return false;

	return true;
}

bool Optimus::Shutdown()
{
	if (World::getInstance()->Shutdown() == false)
		return false;
	return true;
}