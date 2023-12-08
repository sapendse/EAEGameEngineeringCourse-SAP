#include "cLighting.h"
#include "UserInput.h"
#include "cTime.h"

Optimus::cLighting::cLighting() :
m_position( -5.0f, 5.0f, -5.0f ),
m_params( 0.0f, 0.0f, 0.0f, 1.0f ),
m_up ( 0.0f, 1.0f, 0.0f )
{

}

Optimus::cLighting::~cLighting()
{

}

void Optimus::cLighting::setPosition( D3DXVECTOR3& i_position )
{
	m_position = i_position;
}

void Optimus::cLighting::setTranslation( D3DXVECTOR3& i_offset )
{
	m_position += i_offset;
}

void Optimus::cLighting::setParams( D3DXVECTOR4& i_params )
{
	m_params = i_params;
}

void Optimus::cLighting::setAttFac( float i_attFac )
{
	m_attFac = i_attFac;
}

void Optimus::cLighting::Update()
{
	D3DXVECTOR3 offset ( 0.0f, 0.0f, 0.0f );

	if( UserInput::IsKeyPressed( 'I' ) )
		offset.y += 5.0f;
	if( UserInput::IsKeyPressed( 'J' ) )
		offset.x -= 5.0f;
	if( UserInput::IsKeyPressed( 'K' ) )
		offset.y -= 5.0f;
	if( UserInput::IsKeyPressed( 'L' ) )
		offset.x += 5.0f;

	float UPS = 5.0f;

	offset *= ( UPS * g_time.GetSecondsElapsedThisFrame() );
	setTranslation( offset );
}