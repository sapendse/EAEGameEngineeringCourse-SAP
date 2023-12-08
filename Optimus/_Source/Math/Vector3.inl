/* This file is used to define inline functions and constructors of Vector3 class*/

Optimus::Vector3 :: Vector3 ( float i_x, float i_y, float i_z ) : 
	pos_x ( i_x ),
	pos_y ( i_y ),
	pos_z ( i_z)
{
	/*p_myMemPool = NULL;*/
	assert( !isNaN ( i_x ) );
	assert( !isNaN ( i_y ) );
	assert( !isNaN ( i_z ) );
}

Optimus::Vector3 :: Vector3 ( void ) :
	pos_x (0.0),
	pos_y (0.0),
	pos_z (0.0)
{
	/*p_myMemPool = NULL;*/
}

Optimus::Vector3 :: Vector3 ( const Vector3 & rhs ) : 

	pos_x ( rhs.x() ),
	pos_y ( rhs.y() ),
	pos_z ( rhs.z() )

{
	/*p_myMemPool = NULL;*/
	assert( !isNaN ( rhs.pos_x ) );
	assert( !isNaN ( rhs.pos_y ) );
	assert( !isNaN ( rhs.pos_z ) );
}

Optimus::Vector3::Vector3( float i_value ) :
	pos_x( i_value ),
	pos_y( i_value ),
	pos_z( i_value )
{
	assert( !isNaN( i_value ) );
}

void Optimus::Vector3 :: x ( const float& i_x ) {
	assert(!isNaN(i_x));

	pos_x = i_x;
}

void Optimus::Vector3 :: y ( const float& i_y ) {
	
	assert(!isNaN(i_y));

	pos_y=i_y;
}

void Optimus::Vector3 :: z ( const float& i_z ) {
	
	assert(!isNaN(i_z));

	pos_z=i_z;
}

const float Optimus::Vector3 :: x ( void ) const {
	
	return ( pos_x);
}

const float Optimus::Vector3 :: y ( void ) const {
	
	return ( pos_y );
}

const float Optimus::Vector3 :: z ( void ) const {
	
	return ( pos_z );
}

inline const Optimus::Vector3& Optimus::Vector3::operator=( const Vector3& rhs ) {

	assert( !isNaN ( rhs.pos_x ) );
	assert( !isNaN ( rhs.pos_y ) );
	assert( !isNaN ( rhs.pos_z ) );
	
	pos_x= rhs.x();
	pos_y= rhs.y();
	pos_z= rhs.z();

	return *this;
}

inline void  Optimus::Vector3::operator-( ) {

	pos_x=-x();
	pos_y=-y();
	pos_z=-z();
}

inline const Optimus::Vector3 Optimus::operator + ( const Vector3 & lhs , const Vector3 & rhs ) {
	
	return Vector3( lhs.x() + rhs.x() , lhs.y() + rhs.y(), lhs.z() + rhs.z() );
}

inline const Optimus::Vector3 Optimus::operator + ( const Vector3 & lhs , float f ) {

	return Vector3( lhs.x() + f , lhs.y() + f, lhs.z() + f );
}

inline Optimus::Vector3 Optimus::Vector3::getCartesian( const Optimus::Vector3& i_barycentric, float i_p0, float i_p1, float i_p2 ) {
	return Optimus::Vector3( i_barycentric.x() * i_p0, i_barycentric.y() * i_p1, i_barycentric.z() * i_p2 );
}

inline const Optimus::Vector3 Optimus::operator - ( const Vector3 & lhs , const Vector3 & rhs ) {

	return Vector3( lhs.x() - rhs.x() , lhs.y() - rhs.y(), lhs.z() - rhs.z() );
}

inline const Optimus::Vector3 Optimus::operator - ( const Vector3 & lhs , const float f ) {

	return Vector3( lhs.x() - f , lhs.y() - f, lhs.z() - f );
}

inline const Optimus::Vector3 Optimus::operator * ( const Vector3 & lhs , const float f ) {

	return Vector3( lhs.x() * f , lhs.y() * f, lhs.z() * f );
}

inline const Optimus::Vector3 Optimus::operator / ( const  Vector3 & lhs , const float f ) {
	
	if ( !isCloseEnough(f, 0.0f, 0.0001f ) )
		return Vector3( lhs.x() / f , lhs.y() / f, lhs.z() / f );
	else
		return Vector3( 0.0f );
}

const Optimus::Vector3& Optimus::Vector3::operator += ( const Vector3 & rhs ) {
	pos_x += rhs.x();
	pos_y += rhs.y();
	pos_z += rhs.z();

	return *this;
}

const Optimus::Vector3& Optimus::Vector3::operator -= ( const Vector3 & rhs ) {
	
	pos_x -= rhs.x();
	pos_y -= rhs.y();
	pos_z -= rhs.z();

	return *this;
}

const Optimus::Vector3& Optimus::Vector3::operator *= ( const float f ) {
	
	pos_x *= f;
	pos_y *= f;
	pos_z *= f;

	return *this;
}

const Optimus::Vector3& Optimus::Vector3::operator /= ( const float f ) {
	
	if( !( isCloseEnough( f, 0.0f, 0.0001f )) )
	{
		pos_x /= f;
		pos_y /= f;
		pos_z /= f;
	}
	return *this;
}

inline const bool Optimus::operator == ( const Vector3 & lhs, const Vector3 &rhs ) {
	if ( lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z() )
		return true;
	else
		return false;
}

inline const bool Optimus::operator != ( const Vector3 & lhs, const Vector3 &rhs ) {
	if ( lhs.x() != rhs.x() && lhs.y() != rhs.y() && lhs.z() != rhs.z() )
		return true;
	else
		return false;
}

inline const bool Optimus::operator > ( const Vector3 & lhs, const Vector3 &rhs ) {
	if ( Vector3::magSqr(lhs) > Vector3::magSqr(rhs) )
		return true;
	else
		return false;
}

inline const bool Optimus::operator < ( const Vector3 & lhs, const Vector3 &rhs ) {
	if ( Vector3::magSqr(lhs) < Vector3::magSqr(rhs) )
		return true;
	else
		return false;
}

inline const bool Optimus::operator <= ( const Vector3 & lhs, const Vector3 &rhs ) {
	if( lhs.x() <= rhs.x() && lhs.y() <= rhs.y() && lhs.z() <= rhs.z() )
		return true;
	else
		return false;
}

inline const bool Optimus::operator >= ( const Vector3 & lhs, const Vector3 &rhs ) {
	if( lhs.x() >= rhs.x() && lhs.y() >= rhs.y() && lhs.z() >= rhs.z() )
		return true;
	else
		return false;
}

//float Optimus::Vector3 :: dotProd ( const Vector3 & rhs, const Vector3 & lhs ) 

//Optimus::Vector3 Optimus::Vector3 :: crossProd (  const Vector3 & rhs, const Vector3 & lhs ) 

Optimus::Vector3 Optimus::Vector3:: normalise() {
	
	float magni= sqrt ( magSqr(*this) );
	if( !isCloseEnough( magni, 0, 0.0005f ) )
		return Vector3( pos_x/magni, pos_y/ magni, pos_z/magni ); 
	return Vector3( 0.0f,0.0f,0.0f );
}
