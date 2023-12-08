#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "TextField.h"
#include "..\Renderer\cScene.h"
#include "..\Renderer\cSprite.h"
#include "..\Utilities\New.h"

typedef void (*CheckBoxCallback) ( const char* i_menuName, bool i_boolValue );

namespace Optimus
{
	class CheckBox
	{
		CheckBox();
		CheckBox( const char* i_menuName, float i_posX, float i_posY, CheckBoxCallback i_checkBoxCallback = 0 ) :
		m_menuName( i_menuName ),
		m_posX( i_posX ),
		m_posY( i_posY ),
		m_checkBoxCallback( i_checkBoxCallback )
		{
			m_boolValue = false;
			//m_checkBoxText = cSceneHandler::getInstance()->AddText(25, 50, m_menuName, (int)m_posX, (int)m_posY,(int)( m_posX+150 ),(int)( m_posY+25 ), D3DCOLOR_RGBA( 255,255,255,255 ) );
			m_checkBoxText = TextField::CreateTextField( m_menuName,  m_posX, m_posY, false );
			m_checkBoxSprite = cSceneHandler::getInstance()->AddSprite( "../Assets/Textures/checkbox.png", D3DCOLOR_RGBA(255,255,255,255),m_posX+ 260, m_posY - 12.0f );
		}

	public:
		
		const char*			m_menuName;
		float				m_posX, m_posY;
		cSprite*			m_checkBoxSprite;
		TextField*			m_checkBoxText;
		CheckBoxCallback	m_checkBoxCallback;
		bool				m_boolValue;

		static CheckBox* AddCheckBox( const char* i_menuName, float i_posX, float i_posY, CheckBoxCallback i_checkBoxCallback = 0 )
		{
			return OPT_NEW CheckBox( i_menuName, i_posX, i_posY, i_checkBoxCallback );
		}

		void ClickOnCheckBox()
		{
			if( !m_boolValue )
			{
				m_checkBoxSprite->ChangeSpriteTexture( "../Assets/Textures/checkboxClicked.png" );
				m_boolValue = true;
			}
			else
			{
				m_checkBoxSprite->ChangeSpriteTexture( "../Assets/Textures/checkbox.png" );
				m_boolValue = false;
			}

			if( m_checkBoxCallback )
			{
				m_checkBoxCallback( m_menuName, m_boolValue );
			}
		}

		~CheckBox()
		{
			if( m_checkBoxSprite )
			{
				delete m_checkBoxSprite;
				m_checkBoxSprite = NULL;
			}
			if( m_checkBoxText )
			{
				delete m_checkBoxText;
				m_checkBoxText = NULL;
			}
		}
	};
}

#endif