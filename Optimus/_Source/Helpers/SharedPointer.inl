#include <assert.h>
#include "../Utilities/New.h"

namespace Optimus
{
	template< class T >
	SharedPointer<T>::SharedPointer() :
	m_pPtr( 0 ),
	m_pRefCount ( OPT_NEW refcount_t( 1 ) )
	{
	}

	template< class T >
	SharedPointer<T>::SharedPointer( T * i_ptr ) :
	m_pPtr( i_ptr ),
	m_pRefCount ( OPT_NEW refcount_t( 1 ) )
	{
	}

	template< class T >
	SharedPointer<T>::~SharedPointer()
	{
		release();
	}

	template<class T>
	SharedPointer<T>::SharedPointer( const SharedPointer & i_other ) :
	m_pPtr( i_other.m_pPtr ), 
	m_pRefCount( i_other.m_pRefCount ) 
	{ 
		assert( i_other.m_pRefCount );
		
		(*m_pRefCount)++;
	}

	template<class T>
	SharedPointer<T> & SharedPointer<T>::operator=( T * i_other )
	{
		if( m_pPtr != i_other )
		{
			release( );

			m_pPtr = i_other;

			m_pRefCount = OPT_NEW (  refcount_t( 1 ) );
		}

		return *this;
	
	}

	template<class T>
	SharedPointer<T> & SharedPointer<T>::operator=( const SharedPointer & i_other )
	{
		if( this != &i_other )
		{
			release( );
			
			m_pPtr = i_other.m_pPtr;
			
			assert( i_other.m_pRefCount );
			
			m_pRefCount = i_other.m_pRefCount;
			
			(*m_pRefCount)++;
		}
		
		return *this;
	}
	
	template<class T>
	T & SharedPointer<T>::operator*() 
	{
		assert( m_pPtr );
		
		return *m_pPtr;
	}
	
	template<class T>
	T * SharedPointer<T>::operator->()
	{
		return m_pPtr;
	}
	
	template<class T>
	bool SharedPointer<T>::operator==( const T * i_other ) const
	{
		return m_pPtr == i_other;
	}

	template<class T>
	SharedPointer<T>::operator bool() const
	{
		return m_pPtr != 0;
	}

	template<class T>
	void SharedPointer<T>::release()
	{
		assert( m_pRefCount );
		assert( *m_pRefCount > 0 );
		
		if( --(*m_pRefCount) == 0 )
		{
			delete ( m_pRefCount );

			m_pRefCount = 0;
			
			if( m_pPtr )
				delete (m_pPtr);

			m_pPtr = 0;
		}
	}
}