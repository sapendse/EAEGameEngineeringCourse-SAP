#ifndef TARGET_WIN32_H
#define TARGET_WIN32_H

#define CACHE_LINE 64

#define	CACHE_ALIGN __declspec( align( CACHE_LINE ) )

#endif