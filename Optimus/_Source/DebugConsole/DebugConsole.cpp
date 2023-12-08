#include "DebugConsole.h"

#ifdef DEBUG_CONSOLE

Optimus::DebugConsole* Optimus::DebugConsole::m_debugConsoleMenu = 0;

Optimus::DebugConsole::DebugConsole() :
m_curSelection( 0 ),
m_itemCount( 0 ),
m_curDepth( -1 ),
m_showDebugConsole( false )
{
	m_curSelectionSprite = cSceneHandler::getInstance()->AddSprite( "../Assets/Textures/highlight.png", D3DCOLOR_RGBA( 255,255,255,255 ), 0.0f,0.0f );
	m_debugMenuLevel.m_index = 0;
}

int Optimus::DebugConsole::GetType( int i_index )
{
	/*if( m_debugMenuLevel.m_listOfSliders.m_listSliders.find( i_index ) != m_debugMenuLevel.m_listOfSliders.m_listSliders.end() )
		return 1;
	else if( m_debugMenuLevel.m_listOfButtons.m_listButtons.find( i_index ) != m_debugMenuLevel.m_listOfButtons.m_listButtons.end() )
		return 2;
	else if( m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes.find( i_index ) != m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes.end() )
		return 3;*/
	if( m_debugMenuLevel.m_listOfSliders.m_myPosition == m_curSelection )
	{
		if( m_debugMenuLevel.m_listOfSliders.m_depth == m_curDepth )
		{
			return 1;
		}
	}
	if( m_debugMenuLevel.m_listOfButtons.m_myPosition == m_curSelection )
	{
		if( m_debugMenuLevel.m_listOfButtons.m_depth == m_curDepth )
		{
			return 2;
		}
	}
	if( m_debugMenuLevel.m_listOfCheckBoxes.m_myPosition == m_curSelection )
	{
		if( m_debugMenuLevel.m_listOfCheckBoxes.m_depth == m_curDepth )
		{
			return 3;
		}
	}
	return 0;
}

bool Optimus::DebugConsole::GetDebugConsoleState()
{
	return m_showDebugConsole;
}

void Optimus::DebugConsole::InputHandler()
{
	if( m_showDebugConsole )
	{
		if( GetAsyncKeyState(VK_TAB) & 0x8000 == 0x8000 )
		{
			m_curDepth = -1;
			m_showDebugConsole = false;
		}
		else if( GetAsyncKeyState(VK_DOWN) & 0x8000 == 0x8000 )
		{
			if( m_curSelection != m_itemCount - 1 )
			{
				m_curSelection++;
				m_curSelectionSprite->SetPosition( m_curSelectionSprite->GetPosition().x, m_curSelectionSprite->GetPosition().y + 30.0f, m_curSelectionSprite->GetPosition().z );
			}
		}
		else if( GetAsyncKeyState(VK_UP) & 0x8000 == 0x8000 )
		{
			if( m_curSelection != 0 )
			{
				m_curSelection--;
				m_curSelectionSprite->SetPosition( m_curSelectionSprite->GetPosition().x, m_curSelectionSprite->GetPosition().y - 30.0f, m_curSelectionSprite->GetPosition().z );
			}
		}
		else if( GetAsyncKeyState(VK_RETURN) & 0x8000 == 0x8000 )
		{
			int createType = GetType( m_curSelection );
			if( createType == 1 )
			{
				if( m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection] )
				{
					m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection]->m_sliderSprite->SetDrawSprite( !m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection]->m_sliderSprite->GetDrawSprite() );
					m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection]->m_sliderBGSprite->SetDrawSprite( !m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection]->m_sliderBGSprite->GetDrawSprite() );
				}
			}
			else if( createType == 2 )
			{
				if( m_debugMenuLevel.m_listOfButtons.m_listButtons[m_curSelection] )
				{
					m_curDepth++;
					m_curSelectionSprite->SetPosition( m_curSelectionSprite->GetPosition().x + 260.0f, m_curSelectionSprite->GetPosition().y, m_curSelectionSprite->GetPosition().z );
					m_debugMenuLevel.m_listOfButtons.m_listButtons.at( m_curSelection )->ButtonClicked();
				}
			}
			else if( createType == 3 )
			{
				if( m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes[m_curSelection] )
				{
					m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes.at( m_curSelection )->ClickOnCheckBox();
				}
			}
		}
		else if( GetAsyncKeyState(VK_RIGHT) & 0x8000 == 0x8000 )
		{
			int createType = GetType( m_curSelection );
			if( createType == 1 )
				if( m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection] )
					if( m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection]->m_sliderSprite->GetDrawSprite() )
						m_debugMenuLevel.m_listOfSliders.m_listSliders.at( m_curSelection )->SliderValueChange( 1.0f );
		}
		else if( GetAsyncKeyState(VK_LEFT) & 0x8000 == 0x8000 )
		{
			int createType = GetType( m_curSelection );
			if( createType == 1 )
				if( m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection] )
					if( m_debugMenuLevel.m_listOfSliders.m_listSliders[m_curSelection]->m_sliderSprite->GetDrawSprite() )
						m_debugMenuLevel.m_listOfSliders.m_listSliders.at( m_curSelection )->SliderValueChange( -1.0f );
		}
		else if( GetAsyncKeyState( VK_BACK ) & 0x8000 == 0x8000 )
		{
			if( m_curDepth > 0 )
			{
				m_curDepth--;
				m_curSelectionSprite->SetPosition( m_curSelectionSprite->GetPosition().x - 260.0f, m_curSelectionSprite->GetPosition().y, m_curSelectionSprite->GetPosition().z );
			}
		}
	}
	else
	{
		if( GetAsyncKeyState(VK_TAB) & 0x8000 == 0x8000 )
		{
			m_curDepth++;
			m_showDebugConsole = true;
		}
	}
}

void Optimus::DebugConsole::AddSlider( const char* i_menuName, float i_minValue, float i_maxValue, SliderCallback i_sliderCallback )
{
	m_itemCount++;
	std::string tempString = i_menuName;
	if( tempString.find('/') == std::string::npos )
	{
		m_debugMenuLevel.m_listOfSliders.m_depth = 0;
		m_debugMenuLevel.m_listOfSliders.m_associationWith = "NULL";
		m_debugMenuLevel.m_listOfSliders.m_myPosition = m_debugMenuLevel.m_index;
		m_debugMenuLevel.m_index++;
	}
	else
	{
		std::string t1 = tempString.substr( 0, tempString.find("/") );
		tempString = tempString.substr( tempString.find("/") + 1, tempString.length() -1 );
		m_debugMenuLevel.m_listOfSliders.m_depth = 1;
		m_debugMenuLevel.m_listOfSliders.m_associationWith = t1.c_str();
		m_debugMenuLevel.m_listOfSliders.m_myPosition = m_debugMenuLevel.m_index;
		m_debugMenuLevel.m_index++;
		//i_menuName = tempString.c_str();
	}
	int tempDepth = m_debugMenuLevel.m_listOfSliders.m_depth;
	m_debugMenuLevel.m_listOfSliders.m_listSliders.insert( std::pair<int, Slider*>( m_itemCount-1, Slider::CreateSlider( i_menuName, 0.0f+tempDepth * 260, 12.0f + ( m_itemCount - 1 ) * 30.0f, i_minValue, i_maxValue, i_sliderCallback ) ) );
}

void Optimus::DebugConsole::AddCheckbox( const char* i_menuName, CheckBoxCallback i_checkBoxCallback )
{
	m_itemCount++;
	std::string tempString = i_menuName;
	if( tempString.find("/") == std::string::npos )
	{
		m_debugMenuLevel.m_listOfCheckBoxes.m_depth = 0;
		m_debugMenuLevel.m_listOfCheckBoxes.m_associationWith = "NULL";
		m_debugMenuLevel.m_listOfCheckBoxes.m_myPosition = m_debugMenuLevel.m_index;
		m_debugMenuLevel.m_index++;
	}
	else
	{
		std::string t1 = tempString.substr( 0, tempString.find("/") );
		tempString = tempString.substr( tempString.find("/") + 1, tempString.length() -1 );
		m_debugMenuLevel.m_listOfCheckBoxes.m_depth = 1;
		m_debugMenuLevel.m_listOfCheckBoxes.m_associationWith = t1.c_str();
		m_debugMenuLevel.m_listOfCheckBoxes.m_myPosition = m_debugMenuLevel.m_index;
		m_debugMenuLevel.m_index++;
		//i_menuName = tempString.c_str();
	}
	int tempDepth = m_debugMenuLevel.m_listOfCheckBoxes.m_depth;
	m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes.insert( std::pair<int, CheckBox*>( m_itemCount-1,  CheckBox::AddCheckBox( i_menuName, 0.0f+tempDepth * 260, 12.0f + ( m_itemCount - 1 ) * 30.0f, i_checkBoxCallback ) ) );
}

void Optimus::DebugConsole::AddButton( const char* i_menuName, ButtonCallback i_buttonCallback )
{
	m_itemCount++;
	std::string tempString = i_menuName;
	if( tempString.find("/") == std::string::npos )
	{
		m_debugMenuLevel.m_listOfButtons.m_depth = 0;
		m_debugMenuLevel.m_listOfButtons.m_associationWith = "NULL";
		m_debugMenuLevel.m_listOfButtons.m_myPosition = m_debugMenuLevel.m_index;
		m_debugMenuLevel.m_index++;
	}
	else
	{
		std::string t1 = tempString.substr( 0, tempString.find("/") );
		tempString = tempString.substr( tempString.find("/") + 1, tempString.length() -1 );
		m_debugMenuLevel.m_listOfButtons.m_depth = 1;
		m_debugMenuLevel.m_listOfButtons.m_associationWith = t1.c_str();
		m_debugMenuLevel.m_listOfButtons.m_myPosition = m_debugMenuLevel.m_index;
		m_debugMenuLevel.m_index++;
		//i_menuName = tempString.c_str();
	}
	int tempDepth = m_debugMenuLevel.m_listOfButtons.m_depth;
	m_debugMenuLevel.m_listOfButtons.m_listButtons.insert( std::pair<int, Button*>( m_itemCount -1, Button::AddButton( i_menuName, 0.0f +tempDepth * 260, 12.0f + ( m_itemCount - 1 ) * 30.0f, i_buttonCallback ) ) );
}

void Optimus::DebugConsole::AddTextField( const char* i_menuName, const char* i_textToShow )
{
	m_itemCount++;
	std::string tempString = i_menuName;
	if( tempString.find("/") == std::string::npos )
	{
		m_debugMenuLevel.m_texts.m_depth = 0;
		m_debugMenuLevel.m_texts.m_associationWith = "NULL";
	}
	else
	{
		std::string t1 = tempString.substr( 0, tempString.find("/") );
		tempString = tempString.substr( tempString.find("/") + 1, tempString.length() -1 );
		m_debugMenuLevel.m_texts.m_depth = 1;
		m_debugMenuLevel.m_texts.m_associationWith = t1.c_str();
		//i_menuName = tempString.c_str();
	}
	int tempDepth = m_debugMenuLevel.m_texts.m_depth;
	m_debugMenuLevel.m_texts.m_textMenuName.push_back( TextField::CreateTextField( i_menuName, 0.0f+tempDepth * 260, 12.0f + (m_itemCount-1 ) * 30.0f, false ) );
	m_debugMenuLevel.m_texts.m_textToShow.push_back( TextField::CreateTextField( i_textToShow, 50.0f+tempDepth * 260, 12.0f + ( m_itemCount -1 ) * 30.0f, false ) );	
}

void Optimus::DebugConsole::DrawDebugConsole()
{
	m_curSelectionSprite->SetDrawSprite( m_showDebugConsole );
	
	for( unsigned int i= 0; i< m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes.size(); i++ )
	{
		if( m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes[i] && m_curDepth == m_debugMenuLevel.m_listOfCheckBoxes.m_depth )
		{
			m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes[i]->m_checkBoxText->GetText()->SetDrawText( m_showDebugConsole );
			//m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes[i]->m_checkBoxSprite->SetDrawSprite( m_showDebugConsole );
		}
	}

	for( unsigned int i = 0; i< m_debugMenuLevel.m_listOfSliders.m_listSliders.size(); i++ )
	{
		if( m_debugMenuLevel.m_listOfSliders.m_listSliders[i] && m_curDepth == m_debugMenuLevel.m_listOfSliders.m_depth )
		{
			m_debugMenuLevel.m_listOfSliders.m_listSliders[i]->m_sliderText->GetText()->SetDrawText( m_showDebugConsole );
			/*m_debugMenuLevel.m_listOfSliders.m_listSliders[i]->m_sliderSprite->SetDrawSprite( m_showDebugConsole );
			m_debugMenuLevel.m_listOfSliders.m_listSliders[i]->m_sliderBGSprite->SetDrawSprite( m_showDebugConsole );*/
		}
	}

	for( unsigned int i= 0; i< m_debugMenuLevel.m_listOfButtons.m_listButtons.size(); i++ )
	{
		if( m_debugMenuLevel.m_listOfButtons.m_listButtons[i] && m_curDepth == m_debugMenuLevel.m_listOfButtons.m_depth )
		{
			m_debugMenuLevel.m_listOfButtons.m_listButtons[i]->m_buttonText->GetText()->SetDrawText( m_showDebugConsole );
		}
	}

	for( unsigned int i = 0; i < m_debugMenuLevel.m_texts.m_textMenuName.size(); i++ )
	{
		m_debugMenuLevel.m_texts.m_textMenuName[i]->GetText()->SetDrawText( m_showDebugConsole );
		m_debugMenuLevel.m_texts.m_textToShow[i]->GetText()->SetDrawText( m_showDebugConsole );
	}
	
}

void Optimus::DebugConsole::Shutdown()
{
	if( m_debugConsoleMenu )
	{
		delete m_debugConsoleMenu;
		m_debugConsoleMenu = NULL;
	}
}

Optimus::DebugConsole::~DebugConsole()
{
	for( unsigned int i = 0; i < m_debugMenuLevel.m_listOfButtons.m_listButtons.size(); i++ )
	{
		if( m_debugMenuLevel.m_listOfButtons.m_listButtons[i] )
		{
			delete m_debugMenuLevel.m_listOfButtons.m_listButtons[i];
			m_debugMenuLevel.m_listOfButtons.m_listButtons[i] = NULL;
		}
	}

	for( unsigned int i = 0; i< m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes.size(); i++ )
	{
		if( m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes[i] )
		{
			delete m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes[i];
			m_debugMenuLevel.m_listOfCheckBoxes.m_listCheckBoxes[i] = NULL;
		}
	}

	for( unsigned int i = 0; i < m_debugMenuLevel.m_listOfSliders.m_listSliders.size(); i++ )
	{
		if( m_debugMenuLevel.m_listOfSliders.m_listSliders[i] )
		{
			delete m_debugMenuLevel.m_listOfSliders.m_listSliders[i];
			m_debugMenuLevel.m_listOfSliders.m_listSliders[i] = NULL;
		}
	}

	for( unsigned int i = 0; i< m_debugMenuLevel.m_texts.m_textMenuName.size(); i++ )
	{
		if( m_debugMenuLevel.m_texts.m_textMenuName[i] )
		{
			delete m_debugMenuLevel.m_texts.m_textMenuName[i];
			m_debugMenuLevel.m_texts.m_textMenuName[i] = NULL;
		}
	}

	for( unsigned int i = 0; i< m_debugMenuLevel.m_texts.m_textToShow.size(); i++ )
	{
		if( m_debugMenuLevel.m_texts.m_textToShow[i] )
		{
			delete m_debugMenuLevel.m_texts.m_textToShow[i];
			m_debugMenuLevel.m_texts.m_textToShow[i] = NULL;
		}
	}

	if( m_curSelectionSprite )
	{
		delete m_curSelectionSprite;
		m_curSelectionSprite = NULL;
	}
}

#endif