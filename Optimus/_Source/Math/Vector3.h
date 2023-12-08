#ifndef __VECTOR3_H
#define __VECTOR3_H

#include "../MemoryManager/MemoryPool.h"
#include <iostream>
#include <assert.h>
#include <math.h>
#include "..\Utilities\Utilities.h"
#include <d3dx9math.h>

namespace Optimus{

class Vector3 {

	float pos_x, pos_y, pos_z;

public:
	static MemoryPool* p_myMemPool;	

	//Constructors
	inline Vector3 ( void );
	inline Vector3 ( float i_x, float i_y, float i_z );
	inline Vector3 ( const Vector3 & rhs );
	inline Vector3 ( float i_value );
	
	//Setters
	inline void x( const float& i_x );
	inline void y( const float& i_y );
	inline void z( const float& i_z );

	//Getters
	inline const float x( void ) const;
	inline const float y( void ) const;
	inline const float z( void ) const;
	
	//Overloaded Operators in class
	inline const Vector3& operator = ( const Vector3& rhs );
	inline void operator -();
	void* operator new( size_t i_count ){
		if( p_myMemPool == NULL )
			p_myMemPool = MemoryPool::create( i_count, (unsigned int)sizeof(Vector3) );

		return reinterpret_cast<Vector3*>(p_myMemPool->allocate( (p_myMemPool->getSize()) ) );
	}
	
	inline const Vector3& operator += ( const Vector3 & rhs );
	inline const Vector3& operator -= ( const Vector3 & rhs );
	inline const Vector3& operator *= ( const float f );
	inline const Vector3& operator /= ( const float f );

	Vector3 getCartesian( const Vector3& i_barycentric, float i_p0, float i_p1, float i_p2 );

	static void convertToVector3( const D3DXVECTOR3& i_convertFrom, Vector3& i_convertTo )
	{
		assert( !isNaN ( i_convertFrom.x ) );
		assert( !isNaN ( i_convertFrom.y ) );
		assert( !isNaN ( i_convertFrom.z ) );

		i_convertTo.x( i_convertFrom.x );
		i_convertTo.y( i_convertFrom.y );
		i_convertTo.z( i_convertFrom.z );
	}

	static void convertToD3DXVECTOR3( const Vector3& i_convertFrom,  D3DXVECTOR3& i_convertTo )
	{
		assert( !isNaN ( i_convertFrom.x() ) );
		assert( !isNaN ( i_convertFrom.y() ) );
		assert( !isNaN ( i_convertFrom.z() ) );

		i_convertTo.x = i_convertFrom.x();
		i_convertTo.y = i_convertFrom.y();
		i_convertTo.z = i_convertFrom.z();
	}

	void operator delete( void*  i_item ){
		if( i_item )
		{
			unsigned int i_pos= reinterpret_cast< unsigned int >(i_item);
			p_myMemPool->MemoryPool::free_memory( i_pos );
		}
	}
	
	
	//Functions
	static float dotProd ( const Vector3& rhs, const Vector3& lhs )
	{

		assert( !isNaN ( rhs.x() ) );
		assert( !isNaN ( rhs.y() ) );
		assert( !isNaN ( rhs.z() ) );

		assert( !isNaN ( lhs.x() ) );
		assert( !isNaN ( lhs.y() ) );
		assert( !isNaN ( lhs.z() ) );

		return ( rhs.x()*lhs.x() + rhs.y()*lhs.y()+ rhs.z()*lhs.z() );

	}
	static Vector3 crossProd ( const Vector3& rhs, const Vector3& lhs )
	{
		assert( !isNaN ( rhs.x() ) );
		assert( !isNaN ( rhs.y() ) );
		assert( !isNaN ( rhs.z() ) );

		assert( !isNaN ( lhs.x() ) );
		assert( !isNaN ( lhs.y() ) );
		assert( !isNaN ( lhs.z() ) );

		return Vector3( ( (rhs.z()*lhs.y()) - (rhs.y()*lhs.z()) ), ( (rhs.x()*lhs.z()) - (rhs.z()*lhs.x()) ), ( (rhs.y()*lhs.x()) - (rhs.x()*lhs.y()) ) );
	}

	static const Vector3 Zero;

	static float magSqr ( const Vector3 &i_input ){
		float value = i_input.x()*i_input.x() + i_input.y()*i_input.y() + i_input.z()*i_input.z();
		return ( value );
	}
	inline Vector3 normalise();
};

//Overloaded operators outside class
const Vector3 operator + ( const Vector3 & lhs, const Vector3 & rhs );
const Vector3 operator + ( const Vector3 & lhs, float f );
const Vector3 operator - ( const Vector3 & lhs, const Vector3 & rhs );
const Vector3 operator - ( const Vector3 & lhs, float f );
const Vector3 operator * ( const Vector3 & lhs, const float f );
const Vector3 operator / ( const Vector3 & lhs, const float f );

const bool operator == ( const Vector3 & lhs, const Vector3 &rhs );
const bool operator != ( const Vector3 & lhs, const Vector3 &rhs );
const bool operator > ( const Vector3 & lhs, const Vector3 &rhs );
const bool operator < ( const Vector3 & lhs, const Vector3 &rhs );
const bool operator <= ( const Vector3 & lhs, const Vector3 &rhs );
const bool operator >= ( const Vector3 & lhs, const Vector3 &rhs );

const Vector3 v_Zero( 0.0f, 0.0f, 0.0f );
const Vector3 v_Up( 0.0f, 1.0f, 0.0f );
const Vector3 v_Left( -1.0f, 0.0f, 0.0f );
const Vector3 v_Down( 0.0f, -1.0f, 0.0f ); 
const Vector3 v_Right( 1.0f, 0.0f, 0.0f );
const Vector3 v_Forward( 0.0f, 0.0f, 1.0f );

}


#include "Vector3.inl"

#endif