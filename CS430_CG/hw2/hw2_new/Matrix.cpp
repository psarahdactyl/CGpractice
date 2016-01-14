#include "Matrix.h"
#include "Vector.h"

#include <iostream>
#include <cmath>

//3D square matrix 

   	    Vector3D  m[3];
    Matrix3D::Matrix3D( float i = 1.0 ) 
	{
		m[0].x = i; 
		m[1].y = i; 
		m[2].z = i;
	}

    Matrix3D::Matrix3D( const Vector3D& a, const Vector3D& b, const Vector3D& c )
	{ 
		m[0] = a;  
		m[1] = b;  
		m[2] = c;  
	}

    Matrix3D::Matrix3D( float m00, float m10, float m20,
	  float m01, float m11, float m21,
	  float m02, float m12, float m22 ) 
	{
	    m[0] = Vector3D( m00, m01, m02 );
	    m[1] = Vector3D( m10, m11, m12 );
	    m[2] = Vector3D( m20, m21, m22 );
	}

    Matrix3D::Matrix3D( const Matrix3D& n )
	{
	    if ( *this != n ) 
	    {
			m[0] = n.m[0];
			m[1] = n.m[1];
			m[2] = n.m[2];
	    } 
	}

    Vector3D& Matrix3D::operator [] ( int i ) 
    { 
    	return m[i]; 
    }

    const Vector3D& Matrix3D::operator [] ( int i ) const 
    { 
    	return m[i]; 
    }

    Matrix3D Matrix3D::operator + ( const Matrix3D& n ) const
	{ 
		return Matrix3D( m[0]+n[0], m[1]+n[1], m[2]+n[2] ); 
	}

    Matrix3D Matrix3D::operator - ( const Matrix3D& n ) const
	{ 
		return Matrix3D( m[0]-n[0], m[1]-n[1], m[2]-n[2] ); 
	}

    Matrix3D Matrix3D::operator * ( const float s ) const 
	{ 
		return Matrix3D( s*m[0], s*m[1], s*m[2] ); 
	}

    Matrix3D Matrix3D::operator / ( const float s ) const 
    {
		if(s != 0.0)
        {
			float r = float(1.0) / s;
			return *this * r;
		}
    }

    Matrix3D Matrix3D::operator * ( const Matrix3D& n ) const 
    {
		Matrix3D  a( 0.0);

		for ( int i = 0; i < 3; ++i ) 
		{
	  	  	for ( int j = 0; j < 3; ++j ) 
	 	   	{
				for ( int k = 0; k < 3; ++k ) 
				{
		    		a[i][j] += m[i][k] * n[k][j];
				}
	    	}
		}

		return a;
    }

    Matrix3D& Matrix3D::operator += ( const Matrix3D& n ) 
    {
		m[0] += n[0];  m[1] += n[1];  m[2] += n[2]; 
		return *this;
    }

    Matrix3D& Matrix3D::operator -= ( const Matrix3D& n ) 
    {
		m[0] -= n[0];  m[1] -= n[1];  m[2] -= n[2]; 
		return *this;
    }

    Matrix3D& Matrix3D::operator *= ( const float s ) 
    {
		m[0] *= s;  m[1] *= s;  m[2] *= s; 
		return *this;
    }

    Matrix3D& Matrix3D::operator *= ( const Matrix3D& n ) 
    {
		Matrix3D  a( 0.0 );

		for ( int i = 0; i < 3; ++i ) 
		{
	 		for ( int j = 0; j < 3; ++j ) 
	 		{
				for ( int k = 0; k < 3; ++k ) 
				{
		    	a[i][j] += m[i][k] * n[k][j];
				}
	    	}
		}

		return *this = a;
    }

    Matrix3D& Matrix3D::operator /= ( const float s ) 
    {
		if(s != 0.0)
        {
			float r = float(1.0) / s;
			return *this *= r;
		}
    }

    Vector3D Matrix3D::operator * ( const Vector3D& v ) const 
    { 
		return Vector3D( m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
		     m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
		     m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z );
    }

    Matrix3D::operator const float* () const
	{ 
		return static_cast<const float*>( &m[0].x ); 
	}

    Matrix3D::operator float* ()
	{ 
		return static_cast<float*>( &m[0].x ); 
	}

	Matrix3D Matrix3D::matrixCompMult( const Matrix3D& A, const Matrix3D& B ) 
	{
    	return Matrix3D( A[0][0]*B[0][0], A[0][1]*B[0][1], A[0][2]*B[0][2],
		 A[1][0]*B[1][0], A[1][1]*B[1][1], A[1][2]*B[1][2],
		 A[2][0]*B[2][0], A[2][1]*B[2][1], A[2][2]*B[2][2] );
	}

	Matrix3D Matrix3D::transpose( const Matrix3D& A ) 
	{
    	return Matrix3D( A[0][0], A[0][1], A[0][2],
		 A[1][0], A[1][1], A[1][2],
		 A[2][0], A[2][1], A[2][2] );
	}