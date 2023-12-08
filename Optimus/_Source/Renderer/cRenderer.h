/*
	This class defines the renderer class
*/

#ifndef _CRENDERER_H
#define _CRENDERER_H

// Header Files
//=============

#include "Initialization.h"
#include <d3d9.h>
#include <vector>
#include <D3DX9Shader.h>

#include "cScene.h"

// Class Definition
//=================

namespace Optimus
{
	class cRenderer
	{
		// Interface
		//==========

	public:

		// Render
		//-------

		void Update() const;

		// Initialization / Shutdown
		//--------------------------

		cRenderer();
		bool Initialize( int i_width, int i_height, std::string i_sceneName );

		~cRenderer();
		bool Shutdown();
		//void BackGroundColorChange( float i_floatValue );
		

		// Data
		//=====

	private:

		IDirect3D9* m_direct3dInterface;
		IDirect3DDevice9* m_direct3dDevice;
		D3DXVECTOR3 position;
		// Ideally a game would have many different vertex declarations,
		// but we will only use a single one for our class
		IDirect3DVertexDeclaration9* m_vertexDeclaration;
		//cSceneHandler* m_scene;
		// Implementation
		//===============

	private:

		// Initialization / Shutdown
		//--------------------------

		bool InitializeVertexDeclaration();

		bool LoadScene();
		bool ReleaseScene();
	};
}

#endif	// _CRENDERER_H
