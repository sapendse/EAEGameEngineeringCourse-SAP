#ifndef OPTIMUS_H
#define OPTIMUS_H

#include "..\Timer\TimeSystem.h"
#include "..\World\World.h"
#include "..\Renderer\Renderer.h"
#include "..\Actor\ActorController.h"
#include "..\Actor\CollisionController.h"
#include "..\Actor\TriggerCallback.h"
#include "..\Actor\MessageCallback.h"
#include "..\Renderer\UserInput.h"

namespace Optimus{

	bool Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HINSTANCE i_thisInstanceOfTheApplication, int i_initialWindowDisplayState, std::string i_sceneName );
	bool Update();
	bool Shutdown();
}

#endif