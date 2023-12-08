#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include "BitField.h"
#include <iostream>

namespace Optimus{

class MemoryPool
{
	unsigned int m_PoolSize;
	unsigned int m_SizeofData;
	char*		 m_pMemoryBlock;
	BitField*	 pMyBitField;

		// Constructor	
	MemoryPool( char* i_input, unsigned int i_PoolSize, unsigned int i_InputSize ) :
		m_pMemoryBlock	( i_input ),
		m_PoolSize	( i_PoolSize ),
		m_SizeofData ( i_InputSize )
	{
		assert( m_pMemoryBlock != NULL );
		pMyBitField = BitField::create( i_PoolSize );
	}

	//Protecting against copy constructor.
	MemoryPool( MemoryPool &i_copyFrom );

public:
	static MemoryPool* create( unsigned int i_itemNum, unsigned int i_InputSize );
	void *			   allocate( unsigned int i_input );
	void			   free_memory( unsigned int i_position );
	unsigned int	   getSize();
};

}

#endif