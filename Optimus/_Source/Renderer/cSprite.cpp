#include "cSprite.h"

Optimus::cSprite::cSprite( IDirect3DDevice9* i_direct3dDevice, const char* i_textureFilePath, D3DCOLOR i_color, D3DXVECTOR3 i_position ) :
m_direct3dDevice( i_direct3dDevice ),
m_textureFilePath( i_textureFilePath ),
m_color( i_color ),
m_position( i_position ),
m_drawSprite( false )
{
	D3DXCreateTextureFromFile( m_direct3dDevice, i_textureFilePath, &m_spriteTexture );
	D3DXCreateSprite( m_direct3dDevice, &m_sprite );
}

void Optimus::cSprite::ChangeSpriteTexture( const char* i_textureFilePath )
{
	IDirect3DTexture9* tempTex;
	HRESULT res = D3DXCreateTextureFromFile( m_direct3dDevice, i_textureFilePath, &tempTex );

	if( res == D3D_OK )
		m_spriteTexture = tempTex;
}

void Optimus::cSprite::SetPosition( float i_posX, float i_posY, float i_posZ )
{
	m_position.x = i_posX;
	m_position.y = i_posY;
	m_position.z = 0.0f;
}

D3DXVECTOR3 Optimus::cSprite::GetPosition()
{
	return m_position;
}

bool Optimus::cSprite::Draw()
{
	m_sprite->Begin( D3DXSPRITE_ALPHABLEND );
	m_sprite->Draw( m_spriteTexture, NULL, NULL, &m_position, m_color );
	m_sprite->End();

	return true;
}

bool Optimus::cSprite::GetDrawSprite()
{
	return m_drawSprite;
}

void Optimus::cSprite::SetDrawSprite( bool i_boolValue )
{
	m_drawSprite = i_boolValue;
}

Optimus::cSprite::~cSprite()
{
	if( m_sprite )
	{
		m_sprite->Release();
		m_sprite = NULL;
	}
	if( m_spriteTexture )
	{
		m_spriteTexture->Release();
		m_spriteTexture = NULL;
	}
	if( m_direct3dDevice )
	{
		m_direct3dDevice->Release();
		m_direct3dDevice = NULL;
	}
}