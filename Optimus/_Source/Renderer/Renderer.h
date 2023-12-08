#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "..\Actor\GameObject.h"
#include "..\Utilities\Target.h"
#include "..\Helpers\SharedPointer.h"
#include "..\Collision\Collision.h"

namespace Optimus{

	class Renderer{
		std::vector<SharedPointer<GameObject>> m_listOfRenderableObjects;

	public:
		u32 m_scrWidth;
		u32 m_scrHeight;

		static Renderer* m_instance;
		static Renderer* getInstance() 
		{ 
			if( m_instance == 0 )
				m_instance = OPT_NEW Renderer();
			return m_instance; 
		}

		bool addActor( SharedPointer<GameObject> i_gameObject );
		bool removeActor( SharedPointer<GameObject> i_gameObject );
		
		bool Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HWND& i_window, int i_initialWindowDisplayState, std::string i_sceneName );
		bool Initialize( std::string i_gameName, unsigned int i_width, unsigned int i_height, HINSTANCE i_thisInstanceOfTheApplication, int i_initialWindowDisplayState, std::string i_sceneName );
		bool Update( F32 i_deltaTime );
		bool Shutdown();
		bool setScrRes( u32 i_width, u32 i_height );
		void clearActorList();
		void CreateSprite();
	};
}

#endif