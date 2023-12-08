#ifndef BUTTON_H
#define BUTTON_H

#include "TextField.h"
#include "..\Utilities\New.h"

typedef void (*ButtonCallback) ();

namespace Optimus
{
	class Button
	{
		Button();
		Button( const char* i_menuName, float i_posX, float i_posY, ButtonCallback i_buttonCallback ) :
		m_menuName( i_menuName ),
		m_posX( i_posX ),
		m_posY( i_posY ),
		m_buttonCallback( i_buttonCallback )
		{
			m_buttonText = TextField::CreateTextField( m_menuName, m_posX, m_posY - 12.0f, false );
		}

	public:
		const char*			m_menuName;
		float				m_posX, m_posY;
		ButtonCallback		m_buttonCallback;
		TextField*			m_buttonText;

		static Button* AddButton( const char* i_menuName, float i_posX, float i_posY, ButtonCallback i_buttonCallback )
		{
			return OPT_NEW Button( i_menuName, i_posX, i_posY, i_buttonCallback );
		}

		void ButtonClicked()
		{
			m_buttonCallback();
		}

		~Button()
		{
			if( m_buttonText )
			{
				delete m_buttonText;
				m_buttonText = NULL;
			}
		}
	};
}

#endif