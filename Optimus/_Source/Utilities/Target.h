#ifndef TARGET_H
#define TARGET_H

namespace Optimus{

typedef unsigned long long  U64;
typedef float F32;
typedef long long I64;
typedef unsigned _int32 u32;

#if defined _WIN32
#include "Target.Win32.h"
#endif

}

#endif