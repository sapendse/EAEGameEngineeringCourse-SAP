#ifndef __SHARED_POINTER_H
#define __SHARED_POINTER_H

typedef unsigned int refcount_t;

namespace Optimus
{
	template< class T >
	class SharedPointer
	{
		T* m_pPtr;
		refcount_t* m_pRefCount;

		void release();

	public:
		SharedPointer();
		explicit SharedPointer( T * i_ptr );
		SharedPointer( const SharedPointer & i_other );
		SharedPointer & operator=( T * i_other );
		SharedPointer & operator=( const SharedPointer & i_other );

		inline ~SharedPointer();

		T & operator*(); 
		T * operator->();

		bool operator==( const T * i_other ) const;
		operator bool() const;
	};
}

#include "SharedPointer.inl"

#endif