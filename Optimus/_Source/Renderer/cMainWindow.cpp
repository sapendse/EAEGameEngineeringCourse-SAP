// Header Files
//=============

#include "cMainWindow.h"

#include <string>

#include "cTime.h"
#include "Resources/resource.h"

// Static Data Initialization
//===========================

// Window classes are almost always identified by name;
// there is also a unique ATOM associated with them,
// but in practice Windows expects to use the class name as an identifier.
// If you don't change the name below from the default
// your program could have problems when it is run at the same time as one of your classmate's
const char* Optimus::cMainWindow::m_sClassName = "[SAPendse]'s Main Window Class";

namespace
{
	// This text will be displayed in the title bar of the main window
	const char* s_windowCaption = "[SAPendse]'s Renderer";
}

// Helper Function Declarations
//=============================

namespace
{
	std::string GetLastWindowsError();
}

// Interface
//==========

// Access
//-------

HWND Optimus::cMainWindow::GetHandle() const
{
	return m_window;
}

// Initialization / Shutdown
//--------------------------

bool Optimus::cMainWindow::Initialize( int i_width, int i_height, const HINSTANCE i_thisInstanceOfTheApplication, const int i_initialWindowDisplayState, std::string i_sceneName )
{
	m_width = i_width;
	m_height = i_height;

	m_applicationInstance = i_thisInstanceOfTheApplication;

	if ( !RegisterClass() )
	{
		return false;
	}
	if ( !Create( i_initialWindowDisplayState ) )
	{
		return false;
	}

	if ( !m_renderer.Initialize( i_width, i_height, i_sceneName ) )
	{
		return false;
	}

	return true;
}

bool Optimus::cMainWindow::Initialize( int i_width, int i_height, const HWND i_window, const int i_initialWindowDisplayState, std::string i_sceneName )
{
	m_width = i_width;
	m_height = i_height;

	if ( !RegisterClass() )
	{
		return false;
	}
	if ( !Create( i_window, i_initialWindowDisplayState ) )
	{
		return false;
	}

	if ( !m_renderer.Initialize( i_width, i_height, i_sceneName ) )
	{
		return false;
	}

	return true;
}

int Optimus::cMainWindow::WaitForShutdown() const
{
	// Any time something happens that Windows cares about, it will send the main window a message.
	// One of the messages it sends is that the application should quit.
	// This can be sent when a user closes the window,
	// but we can also tell Windows to send a quit message if the user chooses a quit option from within the renderer

	// Enter an infinite loop that will continue until a quit message (WM_QUIT) is received from Windows
	MSG message = { 0 };
	//do
	{
		// To send us a message, Windows will add it to a queue.
		// Most Windows applications should wait until a message is received and then react to it.
		// Real-time renderers though, must continually draw new images to the screen as fast as possible
		// and only pause momentarily when there is a Windows message to deal with.

		// This means that the first thing that must be done every iteration of the render loop is to "peek" at the message queue
		// and see if there are any messages from Windows that need to be handled
		bool hasWindowsSentAMessage;
		{
			HWND getMessagesFromAnyWindowBelongingToTheCurrentThread = NULL;
			unsigned int getAllMessageTypes = 0;
			unsigned int ifAMessageExistsRemoveItFromTheQueue = PM_REMOVE;
			hasWindowsSentAMessage = PeekMessage( &message, getMessagesFromAnyWindowBelongingToTheCurrentThread,
				getAllMessageTypes, getAllMessageTypes, ifAMessageExistsRemoveItFromTheQueue ) == TRUE;
		}
		if ( !hasWindowsSentAMessage )
		{
			// Usually there will be no messages in the queue, and the renderer can run
			g_time.OnNewFrame();
			m_renderer.Update();
		}
		else
		{
			// If Windows _has_ sent a message, this iteration of the loop will handle it

			// First, the message must be "translated"
			// (Key presses are translated into character messages)
			TranslateMessage( &message );

			// Then, the message is sent on to the appropriate processing function.
			// This function is specified in the lpfnWndProc field of the WNDCLASSEX struct
			// used to register a class with Windows.
			// In the case of the main window it will be cMainWindow::OnMessageReceived()
			DispatchMessage( &message );
		}
	} //while ( message.message != WM_QUIT );

	// The exit code for the application is stored in the WPARAM of a WM_QUIT message
	int exitCode = static_cast<int>( message.wParam );

	if( message.message == WM_QUIT )
		return -1;
	return exitCode;
}

Optimus::cMainWindow::~cMainWindow()
{
	Destroy();
	UnregisterClass();
}

// Implementation
//===============

// Initialization / Shutdown
//--------------------------

Optimus::cMainWindow::cMainWindow()
	:
	m_applicationInstance( NULL ), m_class( NULL ), m_window( NULL )
{

}

bool Optimus::cMainWindow::Create( const int i_initialWindowDisplayState )
{
	// The window's style
	DWORD windowStyle =
		// "Overlapped" is basically the same as "top-level"
		WS_OVERLAPPED
		// The caption is the title bar when in windowed-mode
		| WS_CAPTION
		// The window should never change dimensions, so only a minimize box is allowed
		| WS_MINIMIZEBOX
		// The system menu appears when you right-click the title bar
		| WS_SYSMENU;
	// The window's extended style
	DWORD windowStyle_extended =
		// The following is a macro to make the extended style the default top-level look
		WS_EX_OVERLAPPEDWINDOW;
	// The width and height of the window
	// (The renderer is concerned about the width and height of the actual "client area",
	// which is the part of the window that doesn't include the borders and title bar.
	// the initial window, then, will created with default values that Windows chooses
	// and then resized after creation)
	int width = CW_USEDEFAULT;
	int height = CW_USEDEFAULT;
	// The initial position of the window
	// (We don't care, and will let Windows decide)
	int position_x = CW_USEDEFAULT;
	int position_y = CW_USEDEFAULT;
	// Handle to the parent of this window
	// (Since this is our main window, it can't have a parent)
	HWND hParent = NULL;
	// Handle to the menu for this window
	HMENU hMenu = NULL;
	// Handle to the instance of the application this window should be associated with
	HINSTANCE hApplication = m_applicationInstance;
	// Any arbitrary pointer can be associated with this window;
	// usually an application's representation of the window will be used, like this:
	void* userData = this;
	// (Since the main window is a global singleton, though, this program won't actually use this)

	// Ask Windows to create the specified window.
	// CreateWindowEx() will return a handle to the window,
	// which is what we'll use to communicate with Windows about this window
	m_window = CreateWindowEx( windowStyle_extended, m_sClassName, s_windowCaption, windowStyle,
		position_x, position_y, width, height,
		hParent, hMenu, hApplication, userData );
	if ( m_window )
	{
		// Change the window's size based on the requested user settings
		{
			// Calculate how much of the window is coming from the "non-client area"
			// (the borders and title bar)
			RECT windowCoordinates;
			struct
			{
				long width;
				long height;
			} nonClientAreaSize;
			{
				// Get the coordinates of the entire window
				if ( GetWindowRect( m_window, &windowCoordinates ) == FALSE )
				{
					return false;
				}
				// Get the dimensions of the client area
				RECT clientDimensions;
				if ( GetClientRect( m_window, &clientDimensions ) == FALSE )
				{
					return false;
				}
				// Get the difference between them
				nonClientAreaSize.width = ( windowCoordinates.right - windowCoordinates.left ) - clientDimensions.right;
				nonClientAreaSize.height = ( windowCoordinates.bottom - windowCoordinates.top ) - clientDimensions.bottom;
			}
			// Resize the window
			{
				BOOL shouldTheResizedWindowBeRedrawn = TRUE;
				if ( MoveWindow( m_window, windowCoordinates.left, windowCoordinates.top,
					m_width + nonClientAreaSize.width, m_height + nonClientAreaSize.height,
					shouldTheResizedWindowBeRedrawn ) == FALSE )
				{
					return false;
				}
			}
		}

		// Display the window in the initial state that Windows requested
		ShowWindow( m_window, i_initialWindowDisplayState );

		return true;
	}
	else
	{
		return false;
	}
}

bool Optimus::cMainWindow::Create( HWND i_window, const int i_initialWindowDisplayState )
{
	m_window = i_window;
	if ( m_window )
	{
		// Change the window's size based on the requested user settings
		{
			// Calculate how much of the window is coming from the "non-client area"
			// (the borders and title bar)
			RECT windowCoordinates;
			struct
			{
				long width;
				long height;
			} nonClientAreaSize;
			{
				// Get the coordinates of the entire window
				if ( GetWindowRect( m_window, &windowCoordinates ) == FALSE )
				{
					return false;
				}
				// Get the dimensions of the client area
				RECT clientDimensions;
				if ( GetClientRect( m_window, &clientDimensions ) == FALSE )
				{
					return false;
				}
				// Get the difference between them
				nonClientAreaSize.width = ( windowCoordinates.right - windowCoordinates.left ) - clientDimensions.right;
				nonClientAreaSize.height = ( windowCoordinates.bottom - windowCoordinates.top ) - clientDimensions.bottom;
			}
			// Resize the window
			{
				BOOL shouldTheResizedWindowBeRedrawn = TRUE;
				if ( MoveWindow( m_window, windowCoordinates.left, windowCoordinates.top,
					m_width + nonClientAreaSize.width, m_height + nonClientAreaSize.height,
					shouldTheResizedWindowBeRedrawn ) == FALSE )
				{
					return false;
				}
			}
		}

		// Display the window in the initial state that Windows requested
		ShowWindow( m_window, i_initialWindowDisplayState );

		return true;
	}
	else
	{
		return false;
	}
}

bool Optimus::cMainWindow::Destroy()
{
	bool wereThereAnyErrors = false;

	if ( !m_renderer.Shutdown() )
	{
		wereThereAnyErrors = true;
	}

	if ( m_window )
	{
		if ( DestroyWindow( m_window ) == TRUE )
		{
		}
		else
		{
			wereThereAnyErrors = true;
		}
		m_window = NULL;
	}

	return !wereThereAnyErrors;
}

bool Optimus::cMainWindow::RegisterClass()
{
	WNDCLASSEX wndClassEx = { 0 };
	wndClassEx.cbSize = sizeof( WNDCLASSEX );
	wndClassEx.hInstance = m_applicationInstance;

	// The class's style
	// (We don't have to worry about any of these)
	wndClassEx.style = 0;
	// The function that will process all of the messages
	// that Windows will send to windows of this class
	wndClassEx.lpfnWndProc = OnMessageReceived;
	// Extra bytes can be set aside in the class for user data
	// (This is rarely used, but can be useful)
	wndClassEx.cbClsExtra = 0;
	// Extra bytes can be set aside for each window of the class,
	// but this is usually specified for each window individually
	wndClassEx.cbWndExtra = 0;
	// The large and small icons windows of this class should use
	// (These can be found in the Resources folder; feel free to change them)
	wndClassEx.hIcon = LoadIcon( m_applicationInstance, MAKEINTRESOURCE( IDI_RENDERER ) );
	wndClassEx.hIconSm = LoadIcon( m_applicationInstance, MAKEINTRESOURCE( IDI_SMALL ) );
	// The cursor that should display when the mouse pointer is over windows of this class
	wndClassEx.hCursor = LoadCursor( NULL, IDC_ARROW );
	// The brush windows of this class will use as a background
	// (Setting this is a bit confusing, so don't be alarmed if the next line looks scary)
	wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>( IntToPtr( COLOR_BACKGROUND + 1 ) );
	// A menu can be specified that all windows of this class would use by default,
	// but usually this is set for each window individually
	wndClassEx.lpszMenuName = NULL;
	// The class name (see comments where this is initialized)
	wndClassEx.lpszClassName = m_sClassName;

	// Now we can provide this information to Windows.
	// If all goes well, the class will be successfully registered
	// and we can specify it by name when creating our windows.
	m_class = RegisterClassEx( &wndClassEx );
	if ( m_class != NULL )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Optimus::cMainWindow::UnregisterClass()
{
	bool wereThereAnyErrors = false;

	if ( m_class )
	{
		if ( ::UnregisterClass( m_sClassName, m_applicationInstance ) == TRUE )
		{
		}
		else
		{
		}
		m_class = NULL;
	}

	return !wereThereAnyErrors;
}

// Callbacks
//----------

LRESULT CALLBACK Optimus::cMainWindow::OnMessageReceived( HWND i_window, UINT i_message, WPARAM i_wParam, LPARAM i_lParam )
{
	// DispatchMessage() will send messages to the main window here.
	// There are many messages that get sent to a window,
	// but the renderer application can ignore most of them
	// and let Windows handle them in the default way.

	// Process any messages that the renderer cares about
	// (any messages that are processed here should return a value
	// rather than letting the default processing function try to handle them a second time)
	switch( i_message )
	{
	case WM_CHAR:
		{
			// A key has been pressed down, and this is the translated character
			//------------------------------------------------------------------

			// This isn't usually the best way to handle keyboard input,
			// but it is a convenient way to handle an exit key
			if ( i_wParam == VK_ESCAPE )
			{

				int exitCode = 0;
				PostQuitMessage( exitCode );
			}

			break;
		}
	case WM_NCDESTROY:
		{
			// The window's nonclient area is being destroyed
			//-----------------------------------------------


			// This is the last message a window will receive
			// (Any child windows have already been destroyed).
			// After this message has been processed the window's handle will be invalid
			g_mainWindow.m_window = NULL;

			// When the main window is destroyed
			// a WM_QUIT message should be sent
			// (if this isn't done the application would continue to run with no window).
			// This is where the exitCode in cMainWindow::WaitForShutdown() comes from:
			int exitCode = 0;
			PostQuitMessage( exitCode );

			break;
		}
	}

	// Pass any messages that weren't handled on to Windows
	return DefWindowProc( i_window, i_message, i_wParam, i_lParam );
}

// Helper Function Definitions
//============================

namespace
{
	std::string GetLastWindowsError()
	{
		// Windows stores the error as a code
		DWORD errorCode = GetLastError();
		// Format the message
		std::string errorMessage;
		{
			DWORD formattingOptions =
				// Get the error message from Windows
				FORMAT_MESSAGE_FROM_SYSTEM
				// The space for the error message should be allocated by Windows
				| FORMAT_MESSAGE_ALLOCATE_BUFFER
				// Any potentially deferred inserts should be ignored
				// (i.e. the error message should be in its final form)
				| FORMAT_MESSAGE_IGNORE_INSERTS;
			const void* messageIsFromWindows = NULL;
			DWORD useTheDefaultLanguage = 0;
			char* messageBuffer = NULL;
			DWORD minimumCharacterCountToAllocate = 1;
			va_list* insertsAreIgnored = NULL;
			DWORD storedCharacterCount = FormatMessage( formattingOptions, messageIsFromWindows, errorCode,
				useTheDefaultLanguage, reinterpret_cast<LPSTR>( &messageBuffer ), minimumCharacterCountToAllocate, insertsAreIgnored );
			if ( storedCharacterCount != 0 )
			{
				errorMessage = messageBuffer;
			}
			else
			{
				// If there's an error GetLastError() can be called again,
				// but that is too complicated for this program :)
				errorMessage = "Unknown Windows Error";
			}
			// Try to free the memory regardless of whether formatting worked or not,
			// and ignore any error messages
			LocalFree( messageBuffer );
		}
		return errorMessage;
	}
}
