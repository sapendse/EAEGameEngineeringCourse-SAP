#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

namespace Optimus{

inline bool isNaN ( const float val ) ;

inline bool isCloseEnough( float a, float b, float epsilon );

#if defined _DEBUG


#define DEBUG_LEVEL_0 0
#define DEBUG_LEVEL_1 1
#define DEBUG_LEVEL_2 2


inline void printToConsole( int level, const char* fMt , ... );
#endif

#include "Utilities.inl"

}

#endif
