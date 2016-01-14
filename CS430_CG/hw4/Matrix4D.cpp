#include "Matrix4D.h"

#include <iostream>
#include <cmath>

//4D square matrix 

    Matrix4D::Matrix4D( float i = 1.0 ) 
	{
		m[0].x = i; 
		m[1].y = i; 
		m[2].z = i;
		m[3].w = i;
	}

    Matrix4D::Matrix4D( const Vector4D& a, const Vector4D& b, const Vector4D& c, const Vector4D& d )
	{ 
		m[0] = a;  
		m[1] = b;  
		m[2] = c; 
	    m[3] = d;	
	}

    Matrix4D::Matrix4D( float m00, float m10, float m20, float m30,
	  float m01, float m11, float m21, float m31,
	  float m02, float m12, float m22, float m32,
	  float m03, float m13, float m23, float m33 ) 
	{
	    m[0] = Vector4D( m00, m10, m20, m30 );
	    m[1] = Vector4D( m01, m11, m21, m31 );
	    m[2] = Vector4D( m02, m12, m22, m32 );
	    m[3] = Vector4D( m03, m13, m23, m33 );
	}

    Matrix4D::Matrix4D( const Matrix4D& n )
	{
	    if ( *this != n ) 
	    {
			m[0] = n.m[0];
			m[1] = n.m[1];
			m[2] = n.m[2];
			m[3] = n.m[3];
	    } 
	}

    Vector4D& Matrix4D::operator [] ( int i ) 
    { 
    	return m[i]; 
    }

    const Vector4D& Matrix4D::operator [] ( int i ) const 
    { 
    	return m[i]; 
    }

    Matrix4D Matrix4D::operator + ( const Matrix4D& n ) const
	{ 
		return Matrix4D( m[0]+n[0], m[1]+n[1], m[2]+n[2], m[3]+n[3] ); 
	}

    Matrix4D Matrix4D::operator - ( const Matrix4D& n ) const
	{ 
		return Matrix4D( m[0]-n[0], m[1]-n[1], m[2]-n[2], m[3]-n[3] ); 
	}

    Matrix4D Matrix4D::operator * ( const float s ) const 
	{ 
		return Matrix4D( s*m[0], s*m[1], s*m[2], s*m[3] ); 
	}

    Matrix4D Matrix4D::operator / ( const float s ) const 
    {
		if(s != 0.0)
        {
			float r = float(1.0) / s;
			return *this * r;
		}
    }

    Matrix4D Matrix4D::operator * ( const Matrix4D& n ) const 
    {
		Matrix4D  a( 0.0);

		for ( int i = 0; i < 4; ++i ) 
		{
	  	  	for ( int j = 0; j < 4; ++j ) 
	 	   	{
				for ( int k = 0; k < 4; ++k ) 
				{
		    		a[i][j] += m[i][k] * n[k][j];
				}
	    	}
		}

		return a;
    }

    Matrix4D& Matrix4D::operator += ( const Matrix4D& n ) 
    {
		m[0] += n[0];  m[1] += n[1];  m[2] += n[2]; m[3] += n[3];
		return *this;
    }

    Matrix4D& Matrix4D::operator -= ( const Matrix4D& n ) 
    {
		m[0] -= n[0];  m[1] -= n[1];  m[2] -= n[2]; m[3] -= n[3];
		return *this;
    }

    Matrix4D& Matrix4D::operator *= ( const float s ) 
    {
		m[0] *= s;  m[1] *= s;  m[2] *= s; m[3] *= s;
		return *this;
    }

    Matrix4D& Matrix4D::operator *= ( const Matrix4D& n ) 
    {
		Matrix4D  a( 0.0 );

		for ( int i = 0; i < 4; ++i ) 
		{
	 		for ( int j = 0; j < 4; ++j ) 
	 		{
				for ( int k = 0; k < 4; ++k ) 
				{
		    	a[i][j] += m[i][k] * n[k][j];
				}
	    	}
		}

		return *this = a;
    }

    Matrix4D& Matrix4D::operator /= ( const float s ) 
    {
		if(s != 0.0)
        {
			float r = float(1.0) / s;
			return *this *= r;
		}
    }

    Vector4D Matrix4D::operator * ( const Vector4D& v ) const 
    { 
		return Vector4D( m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w,
		     m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w,
		     m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w,
		     m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w );
    }

    Matrix4D::operator const float* () const
	{ 
		return static_cast<const float*>( &m[0].x ); 
	}

    Matrix4D::operator float* ()
	{ 
		return static_cast<float*>( &m[0].x ); 
	}

	Matrix4D Matrix4D::matrixCompMult( const Matrix4D& A, const Matrix4D& B ) 
	{
    	return Matrix4D( A[0][0]*B[0][0], A[0][1]*B[0][1], A[0][2]*B[0][2], A[0][3]*B[0][3],
						A[1][0]*B[1][0], A[1][1]*B[1][1], A[1][2]*B[1][2], A[1][3]*B[1][3],
						A[2][0]*B[2][0], A[2][1]*B[2][1], A[2][2]*B[2][2], A[2][3]*B[2][3],
						A[3][0]*B[3][0], A[3][1]*B[3][1], A[3][2]*B[3][2], A[3][3]*B[3][3] );
	}

	Matrix4D Matrix4D::transpose( const Matrix4D& A ) 
	{
    	return Matrix4D( A[0][0], A[0][1], A[0][2], A[0][3],
		 A[1][0], A[1][1], A[1][2], A[1][3],
		 A[2][0], A[2][1], A[2][2], A[2][3],
		 A[3][0], A[3][1], A[3][2], A[3][3] );
	}
