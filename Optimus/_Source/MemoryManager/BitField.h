#ifndef _BITFIELD_H
#define _BITFIELD_H

#include <iostream>
#include "../Utilities/Target.h"
#include <assert.h>
#include <memory.h>

namespace Optimus{

class BitField
{
	u32 * const	m_pBitField;
	unsigned int		m_numBits;
	unsigned int		m_numElements;

	BitField( u32 * i_input, unsigned int i_numBits, unsigned int i_numElements ) : 
		m_pBitField ( i_input ),
		m_numBits ( i_numBits ),
		m_numElements( i_numElements )
	{
		assert( i_input );
		assert( i_numBits > 0 );

		memset( m_pBitField, 0, (sizeof(u32) *m_numElements) );
	}

	//Protecting against copy constructor.
	BitField( BitField &i_copyFrom );

	public:
	
	~BitField();
	static BitField*	create( unsigned int i_numBits );
	void				setBit( unsigned int i_Position );
	void				clearBit( unsigned int i_position );
	bool				isBitSet( unsigned int i_BitNum );
	bool operator		[]( unsigned int i_Bit );
	unsigned int		getFirstFreeBit( );
	unsigned int		getFirstUsedBit();
	bool				isEmpty();
	static void			BitField_UnitTest();
};

}


#endif