#include "cText.h"
#include "..\Utilities\New.h"

Optimus::cText::cText( IDirect3DDevice9* i_direct3dDevice, const char* i_text, unsigned int i_fontSize, unsigned int i_width, int i_left, int i_top, int i_right, int i_bottom, D3DCOLOR i_color, bool i_drawText ) :
m_text( i_text ),
m_fontSize( i_fontSize ),
m_width( i_width ),
m_color( i_color ),
m_drawText( i_drawText )
{
	m_posRect = OPT_NEW RECT();
	m_posRect->left = i_left;
	m_posRect->top = i_top;
	m_posRect->right = i_right;
	m_posRect->bottom = i_bottom;

	D3DXCreateFont( i_direct3dDevice, i_fontSize, i_width, 100, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );
}

bool Optimus::cText::Draw()
{
	m_font->DrawText( NULL, m_text.c_str(), -1, m_posRect, 0, m_color );

	return true;
}

bool Optimus::cText::GetDrawText()
{
	return m_drawText;
}

void Optimus::cText::SetDrawText( bool i_boolValue )
{
	m_drawText = i_boolValue;
}

void Optimus::cText::SetText( std::string i_newText )
{
	m_text = i_newText;
}

Optimus::cText::~cText()
{
	if( m_posRect )
	{
		delete m_posRect;
		m_posRect = NULL;
	}

	if( m_font )
	{
		m_font->Release();
		m_font = NULL;
	}
}