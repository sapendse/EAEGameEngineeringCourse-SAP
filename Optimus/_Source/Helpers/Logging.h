/*
	This provides very basic functionality for logging messages to a file
*/

#ifndef _LOGGING_H
#define _LOGGING_H

// Header Files
//=============

#include <string>

// Function Declaration
//=====================

namespace Optimus
{
	bool LogMessage( const std::string& i_messageToLog );
}

#endif	//_LOGGING_H
