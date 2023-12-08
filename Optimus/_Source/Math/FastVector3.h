#ifndef __FAST_VECTOR3_H
#define __FAST_VECTOR3_H

#include<iostream>
#include "Vector3.h"
#include "malloc.h"
#include "../Utilities/Target.Win32.h"

namespace Optimus{

CACHE_ALIGN class FastVector3 : public Vector3 {

public:

	inline FastVector3( void ) {};

	inline void * operator new ( size_t size ) 
	{
		return _aligned_malloc( size , CACHE_LINE ) ;
	}

	inline void * operator new[] ( size_t size ) {
		std::cout<<"Size of array: "<<size<< "\n";
		return (FastVector3*)_aligned_malloc( size, __alignof( FastVector3 ) );
	}

	inline void operator delete( void *ptr ) {
		if ( ptr )
			_aligned_free( ptr );
	}
};

}

#endif