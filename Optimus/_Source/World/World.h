/*
Manage the World!
*/
#ifndef _WORLD_H
#define _WORLD_H

#include "..\Actor\Actor.h"
#include "..\Messaging\Messaging.h"
#include "..\Utilities\Target.h"
#include "..\Utilities\New.h"
#include "..\Helpers\SharedPointer.h"
#include "..\Profiling\Profiling.h"
#include "..\Renderer\cScene.h"
#include "..\PathFinding\WayPointGraph.h"
#include <vector>

namespace Optimus{

		class World{
			std::vector<SharedPointer<Actor>> m_listOfWorldObjects;
		public:
			float		m_fps;
			std::string m_fpsString;

			static World* m_instance;
			static World* getInstance()
			{ 	
				if( m_instance == 0 )
				{
					m_instance = OPT_NEW World();
				}
				return m_instance; 
			}
			bool addToWorld( SharedPointer<Actor> i_newActor );
			bool removeFromWorld( SharedPointer<Actor> i_actor );
			void clearList();

			bool Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HINSTANCE i_thisInstanceOfTheApplication, int i_initialWindowDisplayState, std::string i_sceneName );
			bool Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HWND i_window, int i_initialWindowDisplayState, std::string i_sceneName );
			bool Update( F32 deltaTime );
			bool Shutdown();
		};

}

#endif