#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include<assert.h>

namespace Optimus{

	template< class T, unsigned int S >
class RingBuffer
{
	T arr[S];
	int current,count;

public:

	inline RingBuffer( void ) 
	{ 
		current = count = 0; 
	}

	inline void push( const T &input )
	{
		assert( current >= 0);

		arr[ current ] = input;

		current = (++current)% S;
		assert( current < S );

		if( count < S-1 )
			count++;
	}

	inline T pop ( )
	{	
		if( count < 0 ) 
		{
			std::cout<<"Buffer Empty, Nothing to pop!";
			return arr[current];
			//assert( count >=0 );
		}
		else
		{
			if( current > 0 )
				current -= 1;
			else
				current = S-1;

			return arr[current];
			count--;
		}
	}

	inline int getCurrent()	{ return current; }

	unsigned int getSize() const { return S; }

	int getCount() { return count; }

	T operator[]( unsigned int index ) const
	{
		if( index > S )
			index %= S;
		return arr[index];
	}

	inline T getLast( unsigned int number )
	{
		assert( count > 0 );
		int index = (current - number );
		if( index < 0 )
			index = (S)-(number-current);
		return ( arr[index]);
	}
} ;

}

#endif