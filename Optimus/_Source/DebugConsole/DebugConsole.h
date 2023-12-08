#ifndef _DEBUG_CONSOLE_H
#define _DEBUG_CONSOLE_H

#ifdef _DEBUG
#define DEBUG_CONSOLE
#endif

#include <map>
#include "TextField.h"
#include "CheckBox.h"
#include "Button.h"
#include "Slider.h"

namespace Optimus
{

#ifdef DEBUG_CONSOLE
class DebugConsole
{
	int							m_curSelection, m_curDepth, m_itemCount;
	cSprite*					m_curSelectionSprite;
	bool						m_showDebugConsole;
	DebugConsole();

public:
	~DebugConsole();

	static DebugConsole *		m_debugConsoleMenu;

	static DebugConsole*		getInstance()
	{
		if( m_debugConsoleMenu == 0 )
			m_debugConsoleMenu = OPT_NEW DebugConsole();
		return m_debugConsoleMenu;
	}

	/*std::map<int, Slider*>			m_listSliders;
	std::map<int, CheckBox*>		m_listCheckBoxes;
	std::map<int, Button*>			m_listButtons;*/

	
	struct Sliders
	{
	public:
		std::map<int, Slider*>		m_listSliders;
		int							m_depth;
		const char*					m_associationWith;
		int							m_myPosition;
	};

	struct CheckBoxes
	{
	public:
		std::map<int, CheckBox*>	m_listCheckBoxes;
		int							m_depth;
		const char*					m_associationWith;
		int							m_myPosition;
	};

	struct Buttons
	{
	public:
		std::map<int, Button*>		m_listButtons;
		int							m_depth;
		const char*					m_associationWith;
		int							m_myPosition;
	};

	struct Texts
	{		
	public:
		std::vector<TextField*>		m_textMenuName;
		std::vector<TextField*>		m_textToShow;
		int							m_depth;
		const char*					m_associationWith;
	};

	struct DebugMenuLevel
	{
	public:
		Sliders						m_listOfSliders;
		CheckBoxes					m_listOfCheckBoxes;
		Buttons						m_listOfButtons;
		Texts						m_texts;
		int							m_index;
	}m_debugMenuLevel;

	void							AddSlider( const char* i_menuName, float i_minValue, float i_maxValue, void (*Callback) ( const char* i_menuName, float i_floatValue ) = 0 );
	void							AddCheckbox( const char* i_menuName, void (*Callback) ( const char* i_menuName, bool i_boolValue ) = 0 );
	void							AddButton( const char* i_menuName, void (*Callback) () );
	void							AddTextField( const char* i_name, const char* i_textToShow );
	bool							GetDebugConsoleState();
	void							InputHandler();
	int								GetType( int i_index );
	void							DrawDebugConsole();
	void							Shutdown();
};

#else
class DebugConsole
{
	int							m_curSelection, m_curDepth, m_itemCount;
	cSprite*					m_curSelectionSprite;
	bool						m_showDebugConsole;
	DebugConsole();

public:
	static DebugConsole *		m_debugConsoleMenu;

	static DebugConsole*		getInstance()
	{
		return NULL;
	}


	/*std::map<int, Slider*>			m_listSliders;
	std::map<int, CheckBox*>		m_listCheckBoxes;
	std::map<int, Button*>			m_listButtons;*/

	
	struct Sliders
	{
	public:
		std::map<int, Slider*>		m_listSliders;
		int							m_depth;
		const char*					m_associationWith;
		int							m_myPosition;
	};

	struct CheckBoxes
	{
	public:
		std::map<int, CheckBox*>	m_listCheckBoxes;
		int							m_depth;
		const char*					m_associationWith;
		int							m_myPosition;
	};

	struct Buttons
	{
	public:
		std::map<int, Button*>		m_listButtons;
		int							m_depth;
		const char*					m_associationWith;
		int							m_myPosition;
	};

	struct Texts
	{		
	public:
		std::vector<TextField*>		m_textMenuName;
		std::vector<TextField*>		m_textToShow;
		int							m_depth;
		const char*					m_associationWith;
	};

	struct DebugMenuLevel
	{
	public:
		Sliders						m_listOfSliders;
		CheckBoxes					m_listOfCheckBoxes;
		Buttons						m_listOfButtons;
		Texts						m_texts;
		int							m_index;
	}m_debugMenuLevel;


	void							AddSlider( const char* i_menuName, float i_minValue, float i_maxValue, void (*Callback) ( const char* i_menuName, float i_floatValue ) = 0 ) {}
	void							AddCheckbox( const char* i_menuName, void (*Callback) ( const char* i_menuName, bool i_boolValue ) = 0 ) {}
	void							AddButton( const char* i_menuName, void (*Callback) () ) {}
	void							AddTextField( const char* i_name, const char* i_textToShow ) {}
	bool							GetDebugConsoleState() { return false; }
	void							InputHandler() {}
	int								GetType( int i_index ) { return -1; }
	void							DrawDebugConsole() {}
	void							Shutdown() {}
};


#endif
}

#endif