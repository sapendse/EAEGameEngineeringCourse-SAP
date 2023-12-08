#include "Matrix.h"

Optimus::Matrix::Matrix()
{
	for( unsigned int i = 0; i< 4; i++ )
	{
		for( unsigned int j = 0; j< 4; j++ )
		{
			if( i == j )
			{
				m_element[i][j] = 1;
			}
			else
			{
				m_element[i][j] = 0;
			}
		}
	}
}

void Optimus::Matrix::transpose()
{
	for( unsigned int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < 4; j++ )
		{
			if( i < j )
			{
				float temp = m_element[i][j];
				m_element[i][j] = m_element[j][i];
				m_element[j][i] = temp;
			}
		}
	}
}

void Optimus::Matrix::add( Matrix& i_addMatrix )
{
	for( unsigned int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < 4; j++ )
		{
			m_element[i][j] = m_element[i][j] + i_addMatrix.m_element[i][j];
		}
	}
}

void Optimus::Matrix::mult( Matrix& i_multMatrix )
{
	for( unsigned int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < 4; j++ )
		{	
			float sum = 0;
			for( unsigned int k =0; k < 4; k++ )
			{
				sum = sum + m_element[i][k] * i_multMatrix.m_element[k][j];
			}
			m_element[i][j] = sum;
		}
	}
}

void Optimus::Matrix::setRotationMatrix( float i_rotationAngle )
{
	float pi = 3.141529f;
	m_element[0][0] = cos( i_rotationAngle * pi / 180.0f );
	m_element[0][1] = -1 * sin( i_rotationAngle * pi / 180.0f );
	m_element[0][2] = 0;
	m_element[1][0] = sin( i_rotationAngle * pi / 180.0f );
	m_element[1][1] = cos( i_rotationAngle * pi / 180.0f );
	m_element[1][2] = 0;
	m_element[2][0] = 0;
	m_element[2][1] = 0;
	m_element[2][2] = 1;
}

void Optimus::Matrix::setTranslationVector( Vector3& i_translationVector )
{
	//Assume the 1 at m_element[3][3]
	m_element[0][3] = i_translationVector.x();
	m_element[1][3] = i_translationVector.y();
	m_element[2][3] = i_translationVector.z();

}

Optimus::Matrix Optimus::Matrix::getTransformMatrix( float i_rotationAngle, Vector3& i_translationVector )
{
	Matrix temp;
	temp.setRotationMatrix( i_rotationAngle );
	temp.setTranslationVector( i_translationVector );
	temp.m_element[3][0] = temp.m_element[3][1] = temp.m_element[3][2] = 0;
	temp.m_element[3][3] = 1;
	return temp;
}

Optimus::Matrix Optimus::Matrix::getRotationMatrix()
{
	Matrix temp;
	for( unsigned int i = 0; i< 3; i++ )
	{
		for( unsigned int j = 0; j< 3; j++ )
		{
			temp.m_element[i][j] = m_element[i][j];
		}
	}
	return temp;
}

Optimus::Vector3 Optimus::Matrix::getTranslationVector()
{
	Vector3 temp;
	temp.x( m_element[0][3] );
	temp.y( m_element[1][3] );
	temp.z( m_element[2][3] );
	return temp;
}

Optimus::Matrix Optimus::Matrix::inverse()
{
	Vector3 tempTranslation;
	Matrix tempRotation;
	Matrix tempInverse;
	
	tempTranslation = getTranslationVector();
	tempRotation = getRotationMatrix();
	tempRotation.transpose();

	tempTranslation.x( -1 * tempTranslation.x() );
	tempTranslation.y( -1 * tempTranslation.y() );
	tempTranslation.z( -1 * tempTranslation.z() );

	tempTranslation = tempTranslation * tempRotation;
	
	for( unsigned int i = 0; i< 3; i++ )
	{
		for( unsigned int j = 0; j< 3; j++ )
		{
			tempInverse.m_element[i][j] = tempRotation.m_element[i][j];
		}
	}

	tempInverse.setTranslationVector( tempTranslation );

	return tempInverse;
}

Optimus::Matrix Optimus::operator*( Matrix& i_first, Matrix& i_second )
{
	Matrix temp;
	for( unsigned int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < 4; j++ )
		{	
			float sum = 0;
			for( unsigned int k =0; k < 4; k++ )
			{
				sum = sum + i_first.m_element[i][k] * i_second.m_element[k][j];
			}
			temp.m_element[i][j] = sum;
		}
	}
	return temp;
}

Optimus::Matrix Optimus::operator+( Matrix& i_first, Matrix& i_second )
{
	Matrix temp;
	for( unsigned int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < 4; j++ )
		{	
			temp.m_element[i][j] = i_first.m_element[i][j] + i_second.m_element[i][j];
		}
	}
	return temp;
}

Optimus::Vector3 Optimus::operator*( Vector3& i_first, Matrix& i_second )
{
	float tempx=0, tempy=0, tempz=0, tempw=0, sum =0;
	for( unsigned int i = 0; i < 4; i++ )
	{
		sum = i_first.x() * i_second.m_element[i][0] + i_first.y() * i_second.m_element[i][1] + i_first.z() * i_second.m_element[i][2] + 1 * i_second.m_element[i][3];

		if( i == 0 )
			tempx= sum;
		else if( i== 1 )
			tempy = sum;
		else if( i == 2 )
			tempz = sum;
		else if( i == 3 )
			tempw = sum;
	}

	return Vector3( tempx/ tempw, tempy/tempw, tempz/tempw );
}