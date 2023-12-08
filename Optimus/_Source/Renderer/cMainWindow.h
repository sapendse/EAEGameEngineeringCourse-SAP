/*
	This class defines the main application window
*/

#ifndef _CMAINWINDOW_H
#define _CMAINWINDOW_H

// Header Files
//=============

#include "Initialization.h"

#include "cRenderer.h"

// Class Definition
//=================

namespace Optimus
{
	class cMainWindow
	{
		// Interface
		//==========

	public:

		// Singleton Access
		//-----------------

		static inline cMainWindow& GetSingleton()
		{
			static cMainWindow singleton;
			return singleton;
		}
		#define g_mainWindow Optimus::cMainWindow::GetSingleton()

		// Access
		//-------

		HWND GetHandle() const;

		// Initialization / Shutdown
		//--------------------------

		bool Initialize( int i_width, int i_height, const HINSTANCE i_thisInstanceOfTheApplication, const int i_initialWindowDisplayState, std::string i_sceneName );
		bool Initialize( int i_width, int i_height, const HWND i_window, const int i_initialWindowDisplayState, std::string i_sceneName );
		int WaitForShutdown() const;
		~cMainWindow();

		// Data
		//=====

	private:

		HINSTANCE m_applicationInstance;
		ATOM m_class;
		HWND m_window;
		int m_width, m_height;

		cRenderer m_renderer;

		static const char* m_sClassName;

		// Implementation
		//===============

	private:

		// Initialization / Shutdown
		//--------------------------

		cMainWindow();
		bool Create( const int i_initialWindowDisplayState );
		bool Create( HWND i_window, const int i_initialWindowDisplayState );
		bool Destroy();
		bool RegisterClass();
		bool UnregisterClass();

		// Callbacks
		//----------

		static LRESULT CALLBACK OnMessageReceived( HWND i_window, UINT i_message, WPARAM i_wParam, LPARAM i_lParam );

	};
}

#endif	// _CMAINWINDOW_H
