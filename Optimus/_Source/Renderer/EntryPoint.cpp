/*
	The WinMain() function in this file is the entry point of this program
*/

// Header Files
//=============

#include "Initialization.h"

#include <string>

#include "cMainWindow.h"
#include "cTime.h"

// Helper Function Declarations
//=============================

namespace
{
	void LogDateAndTime();
}

// Entry Point
//============

int cInitialise( int i_width, int i_height, HINSTANCE i_thisInstanceOfTheApplication, int i_initialWindowDisplayState, std::string i_sceneName )
{
	// Record the date and time that the program was run so that log files can be differentiated
	LogDateAndTime();

	// Initialize the timer that will keep track of how much time elapses each frame
	if ( !g_time.Initialize() )
	{
		return -1;
	}

	// All of the work will be done in the context of the main window;
	// it must be initialized with the arguments provided by Windows,
	// and then the program will wait until it shuts down
	// and report the resulting error code back to Windows

	int exitCode;
	if( g_mainWindow.Initialize( i_width, i_height, i_thisInstanceOfTheApplication, i_initialWindowDisplayState, i_sceneName ) )
	{
		exitCode = 0;
	}
	else
		exitCode = -1;
	/*{
		exitCode = g_mainWindow.WaitForShutdown();
	}
	else
	{
		exitCode = -1;
	}*/

	return exitCode;
}

int	cInitialise( unsigned int i_width, unsigned int i_height, HWND i_window, int i_initialWindowDisplayState, std::string i_sceneName )
{
	// Record the date and time that the program was run so that log files can be differentiated
	LogDateAndTime();

	// Initialize the timer that will keep track of how much time elapses each frame
	if ( !g_time.Initialize() )
	{
		return -1;
	}

	// All of the work will be done in the context of the main window;
	// it must be initialized with the arguments provided by Windows,
	// and then the program will wait until it shuts down
	// and report the resulting error code back to Windows

	int exitCode;
	if( g_mainWindow.Initialize( i_width, i_height, i_window, i_initialWindowDisplayState, i_sceneName ) )
	{
		exitCode = 0;
	}
	else
		exitCode = -1;
	/*{
		exitCode = g_mainWindow.WaitForShutdown();
	}
	else
	{
		exitCode = -1;
	}*/

	return exitCode;
}

int cUpdate()
{
	int exitCode;
	exitCode = g_mainWindow.WaitForShutdown();
	return exitCode;
}

void cShutDown()
{
	g_mainWindow.~cMainWindow();
}

// Helper Function Definitions
//============================

namespace
{
	void LogDateAndTime()
	{
		std::string message = "Application started at ";

		// Time
		const int bufferLength = 512;
		char buffer[bufferLength];
		{
			LCID useUsersLocale = LOCALE_USER_DEFAULT;
			DWORD useDefaultFormatOptions = 0;
			const SYSTEMTIME* useCurrentLocalTime = NULL;
			const char* useDefaultFormatPicture = NULL;
			int charactersWrittenToBufferCount = GetTimeFormat(
				useUsersLocale, useDefaultFormatOptions, useCurrentLocalTime, useDefaultFormatPicture, buffer, bufferLength );
			message += charactersWrittenToBufferCount > 0 ? buffer : "unknown time";
		}

		message += " on ";

		// Date
		{
			LCID useUsersLocale = LOCALE_USER_DEFAULT;
			DWORD useDefaultFormatOptions = 0;
			const SYSTEMTIME* useCurrentLocalDate = NULL;
			const char* useDefaultFormatPicture = NULL;
			int charactersWrittenToBufferCount = GetDateFormat(
				useUsersLocale, useDefaultFormatOptions, useCurrentLocalDate, useDefaultFormatPicture, buffer, bufferLength );
			message += charactersWrittenToBufferCount > 0 ? buffer : "unknown date";
		}

	}
}
