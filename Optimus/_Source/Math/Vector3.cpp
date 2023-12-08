#include "Vector3.h"

const Optimus::Vector3 Optimus::Vector3::Zero( 0.0f, 0.0f, 0.0f );

float magSqr ( const Optimus::Vector3 &i_input )
{
	float value = i_input.x()*i_input.x() + i_input.y()*i_input.y() + i_input.z()*i_input.z();
	return ( value );
}