// Header Files
//=============

#include "cRenderer.h"

#include <cassert>
#include <d3d9types.h>
#include "cMainWindow.h"
#include "cTime.h"
#include <fstream>
#include "VertexFormat.h"
#include "..\DebugConsole\DebugConsole.h"

// Interface
//==========

// Render
//-------

static D3DCOLOR m_bgColor( D3DCOLOR_XRGB( 0, 0, 255 ) );

void Optimus::cRenderer::Update() const
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		const D3DRECT* subRectanglesToClear = NULL;
		DWORD subRectangleCount = 0;
		DWORD clearColorAndDepth = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
		D3DCOLOR clearColor= D3DCOLOR_XRGB( 0, 0, 0 );
		float clearDepth = 1.0f;
		DWORD noStencilBuffer = 0;
		HRESULT result = m_direct3dDevice->Clear( subRectangleCount, subRectanglesToClear,
			clearColorAndDepth, m_bgColor, clearDepth, noStencilBuffer );
		assert( SUCCEEDED( result ) );
	}
	
	HRESULT result = m_direct3dDevice->BeginScene();
	assert( SUCCEEDED( result ) );


	// The actual function calls that draw geometry must be made between paired calls to
	// BeginScene() and EndScene()
	
	//m_scene->Update();
	//m_scene->Draw();
	cSceneHandler::getInstance()->Update();
	cSceneHandler::getInstance()->Draw();
	
	result = m_direct3dDevice->EndScene();
	assert( SUCCEEDED( result ) );

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be "presented"
	// (to the front buffer)
	{
		const RECT* noSourceRectangle = NULL;
		const RECT* noDestinationRectangle = NULL;
		HWND useDefaultWindow = NULL;
		const RGNDATA* noDirtyRegion = NULL;
		HRESULT result = m_direct3dDevice->Present( noSourceRectangle, noDestinationRectangle, useDefaultWindow, noDirtyRegion );
		assert( SUCCEEDED( result ) );
	}
}

// Initialization / Shutdown
//--------------------------

Optimus::cRenderer::cRenderer()
	:
	position( 0.0f, 0.0f, 0.0f), m_direct3dInterface( NULL ), m_direct3dDevice( NULL ),
	m_vertexDeclaration( NULL )
{
}

	
void ChangeBGColor( const char* i_menuName, float i_floatValue )
{
	int tempColor = (int)i_floatValue * rand();
	m_bgColor = D3DCOLOR_XRGB( tempColor, tempColor * 2 , tempColor *3 );
}


bool Optimus::cRenderer::Initialize(  int i_width, int i_height, std::string i_sceneName )
{
	// Initialize the interface to the Direct3D9 library
	{
		// D3D_SDK_VERSION is #defined by the Direct3D header files,
		// and is just a way to make sure that everything is up-to-date
		m_direct3dInterface = Direct3DCreate9( D3D_SDK_VERSION );
		if ( m_direct3dInterface )
		{
		}
		else
		{
			return false;
		}
	}

	// Create an interface to a Direct3D device
	{
		UINT useDefaultDevice = D3DADAPTER_DEFAULT;
		D3DDEVTYPE useHardwareRendering = D3DDEVTYPE_HAL;
		HWND renderInMainWindow = g_mainWindow.GetHandle();
		DWORD useHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		D3DPRESENT_PARAMETERS presentationParameters = { 0 };
		{
			presentationParameters.BackBufferWidth = i_width;
			presentationParameters.BackBufferHeight = i_height;
			presentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
			presentationParameters.BackBufferCount = 1;
			presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			presentationParameters.hDeviceWindow = g_mainWindow.GetHandle();
			presentationParameters.Windowed = TRUE;
			presentationParameters.EnableAutoDepthStencil = TRUE;
			presentationParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
			presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		HRESULT result = m_direct3dInterface->CreateDevice( useDefaultDevice, useHardwareRendering,
			renderInMainWindow, useHardwareVertexProcessing, &presentationParameters, &m_direct3dDevice );
		if ( SUCCEEDED( result ) )
		{
		}
		else
		{
			return false;
		}
	}

	if ( !InitializeVertexDeclaration() )
	{
		return false;
	}

	//Initialise the Scene
	//if( !m_scene->Initialise( m_direct3dDevice ) )
	if( !cSceneHandler::getInstance()->Initialise( m_direct3dDevice ) )
	{
		return false;
	}

	//if ( !m_scene->parseSceneFile("../Assets/Scenes/sceneinfo.txt") )
	if( !cSceneHandler::getInstance()->parseSceneFile( i_sceneName.c_str() ) )
	{
		return false;
	}

	if( !cSceneHandler::getInstance()->parseDynamicFile( "../WorldEditor/TriggersBin.bin" ) )
	{
		//return false;
	}

	return true;
}

Optimus::cRenderer::~cRenderer()
{
	Shutdown();
}

bool Optimus::cRenderer::Shutdown()
{
	cSceneHandler::getInstance()->ShutDown();
	bool wereThereAnyErrors = false;

	if ( !ReleaseScene() )
	{
		wereThereAnyErrors = true;
	}

	if ( m_vertexDeclaration )
	{
		m_vertexDeclaration->Release();
		m_vertexDeclaration = NULL;
	}

	if ( m_direct3dDevice )
	{
		m_direct3dDevice->Release();
		m_direct3dDevice = NULL;
	}

	if ( m_direct3dInterface )
	{
		m_direct3dInterface->Release();
		m_direct3dInterface = NULL;
	}

	return !wereThereAnyErrors;
}

// Implementation
//===============

// Initialization / Shutdown
//--------------------------

bool Optimus::cRenderer::InitializeVertexDeclaration()
{
	HRESULT result = m_direct3dDevice->CreateVertexDeclaration( s_vertexElements, &m_vertexDeclaration );
	if ( SUCCEEDED( result ) )
	{
		result = m_direct3dDevice->SetVertexDeclaration( m_vertexDeclaration );
		if ( SUCCEEDED( result ) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}



bool Optimus::cRenderer::LoadScene()
{
	
	return true;
}

bool Optimus::cRenderer::ReleaseScene()
{
	/*bool wereThereAnyErrors = false;

	for( unsigned int i=0; i< m_listOfEntities.size();i++ )
	{
		if ( scenes[i].fragmentShaderConstantTable )
		{
			scenes[i].fragmentShaderConstantTable->Release();
			scenes[i].fragmentShaderConstantTable = NULL;
		}
		if ( scenes[i].fragmentShader )
		{
			scenes[i].fragmentShader->Release();
			scenes[i].fragmentShader = NULL;
		}
		if ( scenes[i].vertexShaderConstantTable )
		{
			scenes[i].vertexShaderConstantTable->Release();
			scenes[i].vertexShaderConstantTable = NULL;
		}
		if ( scenes[i].vertexShader )
		{
			scenes[i].vertexShader->Release();
			scenes[i].vertexShader = NULL;
		}
		if ( scenes[i].vertexBuffer )
		{
			scenes[i].vertexBuffer->Release();
			scenes[i].vertexBuffer = NULL;
		}
	}*/
	return true;
}
