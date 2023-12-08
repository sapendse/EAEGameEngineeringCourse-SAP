#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include "..\Renderer\cScene.h"
#include "..\Renderer\cText.h"
#include "..\Utilities\New.h"

namespace Optimus
{
	class TextField
	{
		const char*	m_textToShow;
		float		m_posX, m_posY;
		cText*		m_textToShowText;
		
		TextField();
		TextField( float i_posX, float i_posY, const char* i_textToShow, bool i_drawText ) :
		m_posX( i_posX ),
		m_posY( i_posY ),
		m_textToShow( i_textToShow )
		{
			m_textToShowText = cSceneHandler::getInstance()->AddText( 15, 10, i_textToShow, (int)i_posX, (int)i_posY, (int)i_posX+150, (int)i_posY+25, D3DCOLOR_RGBA( 255, 255, 255, 255 ), i_drawText );
		}
	public:
		static TextField* CreateTextField( const char* i_textToShow, float i_posX, float i_posY, bool i_drawText )
		{
			return OPT_NEW TextField( i_posX, i_posY, i_textToShow, i_drawText );
		}
		
		cText* GetText()
		{
			return m_textToShowText;
		}

		~TextField()
		{
			if( m_textToShowText )
			{
				delete m_textToShowText;
				m_textToShowText = NULL;
			}
		}
	};
}

#endif