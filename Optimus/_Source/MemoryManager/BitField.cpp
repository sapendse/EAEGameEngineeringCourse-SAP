
#include "BitField.h"
#include "..\Utilities\New.h"

Optimus::BitField* Optimus::BitField::create( unsigned int i_numBits )
{
	unsigned int size= i_numBits;
	if( (size % (sizeof(u32)*8)) != 0 )
		size = ( size+ ((sizeof(u32)*8)-1) )/(sizeof(u32)*8);
	u32* pArray = OPT_NEW u32[size];
	if( pArray == NULL )
	{
		std::cout<<"Null array";
		return NULL;
	}
	else
		return OPT_NEW BitField( pArray, i_numBits, size );
}

void Optimus::BitField::setBit( unsigned int i_Position )
{
	unsigned int element = i_Position / (sizeof( u32 )*8);
	unsigned int bit = i_Position % (sizeof(u32)*8);

	m_pBitField[element] |= (1 << bit);
}

void Optimus::BitField::clearBit( unsigned int i_Position )
{
	if( !isEmpty() )
	{
		unsigned int element = i_Position / (sizeof( u32 )*8);
		unsigned int bit = i_Position % (sizeof(u32)*8);

		m_pBitField[element] &= (1<< bit);
	}
}

bool Optimus::BitField::isBitSet( unsigned int i_BitNum )
{
	unsigned int element = i_BitNum / (sizeof(u32)*8);
	unsigned int bit	 = i_BitNum % (sizeof(u32)*8);

	if( m_pBitField[element] & (1<<bit) )
		return true;
	else
		return false;
}

bool Optimus::BitField::operator[]( unsigned int i_Bit )
{
	return ( isBitSet( i_Bit ) );
}

unsigned int Optimus::BitField::getFirstFreeBit()
{
	unsigned int element = 0;
	
	//Find element with alteast 1 free bit.
	while( m_pBitField[element] == UINT_MAX && element < m_numElements )
		element++;

	if( element == m_numElements )
		return (UINT_MAX-1);

	unsigned int bit = 0;

	//Find the bit inside that element.
	while( bit < (sizeof( u32) *8)-1 )
	{
		if( !isBitSet(bit) )
			return (element*(sizeof(u32)*8)+bit);
		bit++;
	}
	return (UINT_MAX -1);
}

unsigned int Optimus::BitField::getFirstUsedBit()
{
	unsigned int element = 0;
	unsigned int empty =0;

	//Find the element with empty bit(s).
	while( m_pBitField[element] != empty && element< m_numElements-1 && m_pBitField[element] < UINT_MAX )
		element++;

	if( element == m_numElements )
		return (UINT_MAX-1);

	unsigned int bit =0;
	//Find first empty bit.
	while( bit < (sizeof(u32)*8)-1)
	{
		if( isBitSet(bit) )
			return (element*8*(sizeof(u32))+bit );
		bit++;
	}
	return (UINT_MAX -1);
}

bool Optimus::BitField::isEmpty()
{
	if( m_numBits == 0 )
		return true;
	else
		return false;
}

void Optimus::BitField::BitField_UnitTest()
{
	BitField * pMyTestBitField= BitField::create( 33 );

	assert( pMyTestBitField >0 );

	std::cout<<"\nElements of Bit Field: \n";
	for(unsigned int i=0; i< pMyTestBitField->m_numElements;i++)
	{
		std::cout<<"\nNumber: "<<i<<" \nValue: "<<pMyTestBitField->m_pBitField[i]<<"\n";
	}

	unsigned int firstFreeLocation = pMyTestBitField->getFirstFreeBit();
	assert( firstFreeLocation != UINT_MAX-1 );
	std::cout<<"Empty BitField, Answer should be 0. It is: "<<firstFreeLocation;

	if( pMyTestBitField->isBitSet( 0 ) )
		std::cout<<"\nThere is a problem"; // shouldn't print.
	

}