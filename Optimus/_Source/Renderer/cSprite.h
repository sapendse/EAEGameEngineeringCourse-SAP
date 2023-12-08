#ifndef CSPRITE_H
#define CSPRITE_H

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9math.h>

namespace Optimus{

	class cSprite
	{
		const char*			m_textureFilePath;
		IDirect3DTexture9*  m_spriteTexture;
		IDirect3DDevice9*	m_direct3dDevice;
		ID3DXSprite*		m_sprite;
		D3DCOLOR			m_color;
		D3DXVECTOR3			m_position;
		bool				m_drawSprite;

	public:
		//Constructor
		cSprite( IDirect3DDevice9* i_direct3dDevice, const char* i_textureFilePath, D3DCOLOR i_color, D3DXVECTOR3 i_position );
		~cSprite();

		void				SetPosition( float i_posX, float i_posY, float i_posZ );
		D3DXVECTOR3			GetPosition();
		void				ChangeSpriteTexture( const char* i_textureFilePath );
		bool				Draw();
		bool				GetDrawSprite();
		void				SetDrawSprite( bool i_boolValue );
		void				Shutdown();
	};

}
#endif