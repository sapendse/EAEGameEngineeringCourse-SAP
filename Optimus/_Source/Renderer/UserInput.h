/*
	This class manages user input from the keyboard or mouse
*/

#ifndef _USERINPUT_H
#define _USERINPUT_H

// Header Files
//=============

#include "Initialization.h"

// Interface
//==========

namespace Optimus
{
	namespace UserInput
	{
		// As of this comment, the codes for these functions can be found at:
		// http://msdn.microsoft.com/en-us/library/dd375731(v=vs.85).aspx
		// (if that doesn't work, try searching for "Virtual Key Codes")

		// For standard letter or number keys, the representative ascii char can be used:
		// IsKeyPressed( 'A' ) or IsKeyPressed( '6' )

		bool IsKeyPressed( const int i_virtualKeyCode );
		bool IsButtonPressed( const int i_virtualButtonCode );
	}
}

#endif	// _USERINPUT_H
