#include <iostream>
#include "MemoryPool.h"
#include "../Utilities/New.h"

Optimus::MemoryPool* Optimus::MemoryPool::create( unsigned int i_itemNum, unsigned int i_InputSize )
{
	char* pArray= OPT_NEW char[i_itemNum];

	if( pArray != NULL )
		return OPT_NEW MemoryPool( pArray, i_itemNum, i_InputSize );
	else
		return NULL;
}

void * Optimus::MemoryPool::allocate( unsigned int i_InputSize )
{
	assert( i_InputSize <= m_PoolSize );

	unsigned int bit = pMyBitField->getFirstFreeBit();

	assert( bit != (UINT_MAX-1) );
	pMyBitField->setBit(bit);

	return (m_pMemoryBlock+ (bit*m_SizeofData)) ;
}

void Optimus::MemoryPool::free_memory( unsigned int i_position )
{
	assert( i_position < m_PoolSize );
	if( !(pMyBitField->isEmpty()) )
	{
		unsigned int bit = i_position % ( sizeof( u32 )*8 );
		pMyBitField->clearBit( i_position );
	}
}

unsigned int Optimus::MemoryPool::getSize()
{
	return m_SizeofData;
}