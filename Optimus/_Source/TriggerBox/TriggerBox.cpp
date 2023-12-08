#include "TriggerBox.h"
#include "..\Utilities\New.h"

Optimus::TriggerBox::TriggerBox() :
m_center( Optimus::v_Zero ),
m_dimensions( Optimus::v_Zero ),
line_vertices( NULL ),
m_drawable( true )
{

}

Optimus::TriggerBox::TriggerBox( Vector3& i_center, Vector3& i_dimensions ) :
m_center( i_center ),
m_dimensions( i_dimensions )
{

}

Optimus::TriggerBox::TriggerBox( TriggerBox& i_rhs ) :
m_center( i_rhs.GetCenter() ),
m_dimensions( i_rhs.GetDimensions() )
{

}

void Optimus::TriggerBox::SetDrawable( bool i_drawable )
{
	m_drawable = i_drawable;
}

void Optimus::TriggerBox::Initialize()
{
	line_vertices = OPT_NEW Line[16];
}

Optimus::Vector3 Optimus::TriggerBox::GetCenter()
{
	return m_center;
}

Optimus::Vector3 Optimus::TriggerBox::GetDimensions()
{
	return m_dimensions;
}

void Optimus::TriggerBox::SetPosition( const Optimus::Vector3& i_position )
{
	m_center = i_position;
}

void Optimus::TriggerBox::Draw( IDirect3DDevice9* i_direct3dDevice )
{
	if( m_drawable )
	{
		Optimus::Vector3 minBound = m_center - ( m_dimensions / 2 );
		Optimus::Vector3 maxBound = m_center + ( m_dimensions / 2 );

		const DWORD line_fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

		line_vertices[0].x = minBound.x();
		line_vertices[0].y = minBound.y();
		line_vertices[0].z = minBound.z();
		line_vertices[0].color = D3DCOLOR_XRGB (0, 255, 255);
			
		line_vertices[1].x = maxBound.x();
		line_vertices[1].y = minBound.y();
		line_vertices[1].z = minBound.z();
		line_vertices[1].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[2].x = maxBound.x();
		line_vertices[2].y = maxBound.y();
		line_vertices[2].z = minBound.z();
		line_vertices[2].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[3].x = minBound.x();
		line_vertices[3].y = maxBound.y();
		line_vertices[3].z = minBound.z();
		line_vertices[3].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[4].x = minBound.x();
		line_vertices[4].y = minBound.y();
		line_vertices[4].z = minBound.z();
		line_vertices[4].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[5].x = minBound.x();
		line_vertices[5].y = minBound.y();
		line_vertices[5].z = maxBound.z();
		line_vertices[5].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[6].x = minBound.x();
		line_vertices[6].y = maxBound.y();
		line_vertices[6].z = maxBound.z();
		line_vertices[6].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[7].x = minBound.x();
		line_vertices[7].y = maxBound.y();
		line_vertices[7].z = minBound.z();
		line_vertices[7].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[8].x = maxBound.x();
		line_vertices[8].y = maxBound.y();
		line_vertices[8].z = minBound.z();
		line_vertices[8].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[9].x = maxBound.x();
		line_vertices[9].y = maxBound.y();
		line_vertices[9].z = maxBound.z();
		line_vertices[9].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[10].x = minBound.x();
		line_vertices[10].y = maxBound.y();
		line_vertices[10].z = maxBound.z();
		line_vertices[10].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[11].x = minBound.x();
		line_vertices[11].y = minBound.y();
		line_vertices[11].z = maxBound.z();
		line_vertices[11].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[12].x = maxBound.x();
		line_vertices[12].y = minBound.y();
		line_vertices[12].z = maxBound.z();
		line_vertices[12].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[13].x = maxBound.x();
		line_vertices[13].y = minBound.y();
		line_vertices[13].z = minBound.z();
		line_vertices[13].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[14].x = maxBound.x();
		line_vertices[14].y = minBound.y();
		line_vertices[14].z = maxBound.z();
		line_vertices[14].color = D3DCOLOR_XRGB (0, 255, 255);

		line_vertices[15].x = maxBound.x();
		line_vertices[15].y = maxBound.y();
		line_vertices[15].z = maxBound.z();
		line_vertices[15].color = D3DCOLOR_XRGB (0, 255, 255);

		i_direct3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 15, line_vertices, sizeof(Line));
	}
}

Optimus::TriggerBox::~TriggerBox()
{
	Shutdown();
}

void Optimus::TriggerBox::Shutdown()
{
	if( line_vertices != NULL )
	{
		delete line_vertices;
		line_vertices = NULL;
	}
}

void Optimus::TriggerBox::OnEnter( Optimus::SharedPointer<Optimus::GameObject> i_enter, Optimus::SharedPointer<Optimus::GameObject> i_trigger )
{
	
}

void Optimus::TriggerBox::OnExit( Optimus::SharedPointer<Optimus::GameObject> i_enter, Optimus::SharedPointer<Optimus::GameObject> i_trigger )
{
	
}