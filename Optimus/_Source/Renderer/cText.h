#ifndef CTEXT_H
#define CTEXT_H

#include <Windows.h>
#include <WinDef.h>
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9core.h>
#include <string>

namespace Optimus
{
	class cText
	{
		std::string			m_text;
		unsigned int		m_fontSize, m_width;
		RECT*				m_posRect;
		ID3DXFont*			m_font;
		D3DCOLOR			m_color;
		bool				m_drawText;

	public:
		cText( IDirect3DDevice9* i_direct3dDevice, const char* i_text, unsigned int i_fontSize, unsigned int i_width, int i_left, int i_top, int i_right, int i_bottom, D3DCOLOR i_color, bool i_drawText );
		~cText();

		bool				Draw();
		bool				GetDrawText();
		void				SetDrawText( bool i_boolValue );
		void				SetText( std::string i_newText );
	};
}

#endif