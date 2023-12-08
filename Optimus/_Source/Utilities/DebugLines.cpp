#include "DebugLines.h"



void Optimus::DebugLines::Initialize()
{
	line_vertices = OPT_NEW Line[16];
	raycastPoints = OPT_NEW Line[2];
}

void Optimus::DebugLines::DrawOctreeBox( IDirect3DDevice9* i_direct3dDevice )
{
	//const DWORD line_fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	line_vertices[0].x = m_minBound.x();
	line_vertices[0].y = m_minBound.y();
	line_vertices[0].z = m_minBound.z();
	line_vertices[0].color = m_color;
			
	line_vertices[1].x = m_maxBound.x();
	line_vertices[1].y = m_minBound.y();
	line_vertices[1].z = m_minBound.z();
	line_vertices[1].color = m_color;

	line_vertices[2].x = m_maxBound.x();
	line_vertices[2].y = m_maxBound.y();
	line_vertices[2].z = m_minBound.z();
	line_vertices[2].color = m_color;

	line_vertices[3].x = m_minBound.x();
	line_vertices[3].y = m_maxBound.y();
	line_vertices[3].z = m_minBound.z();
	line_vertices[3].color = m_color;

	line_vertices[4].x = m_minBound.x();
	line_vertices[4].y = m_minBound.y();
	line_vertices[4].z = m_minBound.z();
	line_vertices[4].color = m_color;

	line_vertices[5].x = m_minBound.x();
	line_vertices[5].y = m_minBound.y();
	line_vertices[5].z = m_maxBound.z();
	line_vertices[5].color = m_color;

	line_vertices[6].x = m_minBound.x();
	line_vertices[6].y = m_maxBound.y();
	line_vertices[6].z = m_maxBound.z();
	line_vertices[6].color = m_color;

	line_vertices[7].x = m_minBound.x();
	line_vertices[7].y = m_maxBound.y();
	line_vertices[7].z = m_minBound.z();
	line_vertices[7].color = m_color;

	line_vertices[8].x = m_maxBound.x();
	line_vertices[8].y = m_maxBound.y();
	line_vertices[8].z = m_minBound.z();
	line_vertices[8].color = m_color;

	line_vertices[9].x = m_maxBound.x();
	line_vertices[9].y = m_maxBound.y();
	line_vertices[9].z = m_maxBound.z();
	line_vertices[9].color = m_color;

	line_vertices[10].x = m_minBound.x();
	line_vertices[10].y = m_maxBound.y();
	line_vertices[10].z = m_maxBound.z();
	line_vertices[10].color = m_color;

	line_vertices[11].x = m_minBound.x();
	line_vertices[11].y = m_minBound.y();
	line_vertices[11].z = m_maxBound.z();
	line_vertices[11].color = m_color;

	line_vertices[12].x = m_maxBound.x();
	line_vertices[12].y = m_minBound.y();
	line_vertices[12].z = m_maxBound.z();
	line_vertices[12].color = m_color;

	line_vertices[13].x = m_maxBound.x();
	line_vertices[13].y = m_minBound.y();
	line_vertices[13].z = m_minBound.z();
	line_vertices[13].color = m_color;

	line_vertices[14].x = m_maxBound.x();
	line_vertices[14].y = m_minBound.y();
	line_vertices[14].z = m_maxBound.z();
	line_vertices[14].color = m_color;

	line_vertices[15].x = m_maxBound.x();
	line_vertices[15].y = m_maxBound.y();
	line_vertices[15].z = m_maxBound.z();
	line_vertices[15].color = m_color;

	i_direct3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 15, line_vertices, sizeof(Line));
}

void Optimus::DebugLines::DrawForwardRaycast( IDirect3DDevice9* i_direct3dDevice )
{	
	//const DWORD line_fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
		
	raycastPoints[0].x = m_start.x();
	raycastPoints[0].y = m_start.y() + 40.0f;
	raycastPoints[0].z = m_start.z();
	raycastPoints[0].color = m_color;

	raycastPoints[1].x = m_end.x();
	raycastPoints[1].y = m_end.y() + 40.0f;
	raycastPoints[1].z = m_end.z();
	raycastPoints[1].color = m_color;

	i_direct3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, raycastPoints, sizeof(Line));
}

void Optimus::DebugLines::Draw( IDirect3DDevice9* i_direct3dDevice )
{
	if( m_minBound == Optimus::v_Zero )
	{
		DrawForwardRaycast( i_direct3dDevice );
	}
	else if( m_start == Optimus::v_Zero )
	{
		DrawOctreeBox( i_direct3dDevice );
	}
}

Optimus::DebugLines::~DebugLines()
{
	Shutdown();
}

void Optimus::DebugLines::Shutdown()
{
	if( line_vertices )
	{
		delete[] line_vertices; 
		line_vertices = NULL;
	}
	if( raycastPoints )
	{
		delete[]  raycastPoints;
		raycastPoints = NULL;
	}
}
