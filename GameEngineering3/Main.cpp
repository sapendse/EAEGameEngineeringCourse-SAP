#include "Game.h"

Game* Game::m_instance = NULL;

int WINAPI WinMain( HINSTANCE i_thisInstanceOfTheApplication, HINSTANCE, char* i_commandLineArguments, int i_initialWindowDisplayState )
{
		// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 777 );
#endif

	Optimus::Initialize( "MyGame", 900, 600, i_thisInstanceOfTheApplication, i_initialWindowDisplayState, "../Assets/Scenes/MainScene.txt" );
	Game::GetInstance()->Initialize();

	while( true )
	{
		if( Game::GetInstance()->Update() == false )
			break;
		if( Optimus::Update() == false )
			break;
	}

	Game::GetInstance()->ShutDown();
	Optimus::Shutdown();

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}