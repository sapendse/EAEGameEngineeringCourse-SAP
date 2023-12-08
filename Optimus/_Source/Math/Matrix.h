#ifndef MATRIX_H
#define MATRIX_H

#include <assert.h>
#include <math.h>
#include "Vector3.h"

namespace Optimus
{
	class Matrix
	{
	public:		
		
		float m_element[4][4];
		
		//Constructor and Destructor
		Matrix();
		~Matrix() { }

		//Functions
		void transpose();		
		void add( Matrix& i_addMatrix );
		void mult( Matrix& i_multMatrix );

		//Assume the form R T
		//				  0 1
		void setRotationMatrix( float i_rotationAngle );
		void setTranslationVector( Vector3& i_translationVector );
		static Matrix getTransformMatrix( float i_rotationAngle, Vector3& i_translationVector );
		Matrix getRotationMatrix();
		Vector3 getTranslationVector();
		Matrix inverse();
	};

	Matrix operator*( Matrix& i_first, Matrix & i_second );
	Vector3 operator*( Vector3& i_first, Matrix& i_second );
	Matrix operator+( Matrix& i_first, Matrix& i_second );
}

#endif