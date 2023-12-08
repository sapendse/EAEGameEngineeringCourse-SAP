#ifndef DEBUG_LINES_H
#define DEBUG_LINES_H

#include "New.h"
#include "..\Math\Vector3.h"
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <d3dx9math.h>

namespace Optimus
{

	struct Line
	{
		float x, y, z;
		DWORD color;
	};

	class DebugLines
	{
		Optimus::Vector3	m_minBound, m_maxBound, m_start, m_end;
		Line*				line_vertices;
		Line*				raycastPoints;
		DWORD				m_color;

	public :

		//Constructor
		DebugLines( const Optimus::Vector3& i_minBound, const Optimus::Vector3& i_maxBound, const Optimus::Vector3& i_start, const Optimus::Vector3& i_end, DWORD i_color )
 		{
			m_minBound = i_minBound;
			m_maxBound = i_maxBound;
			m_start = i_start;
			m_end = i_end;
			m_color = i_color;
		}

		DebugLines() : m_minBound( Optimus::v_Zero ), m_maxBound( Optimus::v_Zero ), m_start( Optimus::v_Zero ), m_end( Optimus::v_Zero )
		{}

		//Destructor
		~DebugLines();
		
		//Functions
		void Initialize();
		void DrawOctreeBox( IDirect3DDevice9* i_direct3dDevice );
		void DrawForwardRaycast( IDirect3DDevice9* i_direct3dDevice );
		void Draw( IDirect3DDevice9* i_direct3dDevice );
		void Shutdown();
	};
}

#endif