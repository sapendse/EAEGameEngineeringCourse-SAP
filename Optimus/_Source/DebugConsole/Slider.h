#ifndef SLIDER_H
#define SLIDER_H

#include "TextField.h"
#include "..\Renderer\cSprite.h"
#include "..\Renderer\cScene.h"

typedef void (*SliderCallback) ( const char* i_menuName, float m_floatValue );

namespace Optimus
{
	class Slider
	{
		Slider();
		Slider( const char* i_menuName, float i_posX, float i_posY, float i_minValue, float i_maxValue, SliderCallback i_sliderCallback = 0 ) :
		m_menuName( i_menuName ),
		m_posX( i_posX ),
		m_posY( i_posY ),
		m_minValue( i_minValue ),
		m_maxValue( i_maxValue ),
		m_sliderCallback( i_sliderCallback )
		{
			m_curValue = 0.0f;
			m_offset = m_posX + 260.0f;
			m_sliderText = TextField::CreateTextField( i_menuName, i_posX, i_posY, false );
			m_sliderBGSprite = cSceneHandler::getInstance()->AddSprite( "../Assets/Textures/sliderBG.png", D3DCOLOR_RGBA( 255,255,255,255 ), m_posX + 260.0f, m_posY - 12.0f );
			m_sliderSprite = cSceneHandler::getInstance()->AddSprite( "../Assets/Textures/slider.png", D3DCOLOR_RGBA( 255,255,255,255 ), m_posX + 260.0f, m_posY - 12.0f );
		}

	public:
		const char*		m_menuName;
		float			m_curValue, m_minValue, m_maxValue, m_posX, m_posY, m_offset;
		SliderCallback	m_sliderCallback;
		TextField*		m_sliderText;
		cSprite*		m_sliderBGSprite;
		cSprite*		m_sliderSprite;

		static Slider* CreateSlider( const char* i_menuName, float i_posX, float i_posY, float i_minValue, float i_maxValue, SliderCallback i_sliderCallback = 0 )
		{
			return  OPT_NEW Slider( i_menuName, i_posX, i_posY, i_minValue, i_maxValue, i_sliderCallback );
		}

		void SliderValueChange( float i_value )
		{
			if( m_curValue <= m_maxValue )
			{
				m_curValue += i_value;
				if( m_curValue > m_maxValue )
					m_curValue = m_maxValue;
			}

			if( m_curValue >= m_minValue )
			{
				m_curValue += i_value;
				if( m_curValue < m_minValue )
					m_curValue = m_minValue;
			}


			m_sliderSprite->SetPosition( m_offset + m_curValue, m_sliderSprite->GetPosition().y, m_sliderSprite->GetPosition().z );

			if( m_sliderCallback )
				m_sliderCallback( m_menuName, m_curValue );
		}

		~Slider()
		{
			if( m_sliderText )
			{
				delete m_sliderText;
				m_sliderText = NULL;
			}
			if( m_sliderBGSprite )
			{
				delete m_sliderBGSprite;
				m_sliderBGSprite = NULL;
			}
			if( m_sliderSprite )
			{
				delete m_sliderSprite;
				m_sliderSprite = NULL;
			}
		}
	};
}

#endif