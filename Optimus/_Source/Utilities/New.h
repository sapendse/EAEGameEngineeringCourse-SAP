#ifndef __NEW_H
#define __NEW_H

#ifdef _DEBUG
#include <crtdbg.h>
#define OPT_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define OPT_NEW new
#endif

#endif  // __NEW_H