#ifndef MATRIX3D_H
#define MATRIX3D_H

#include "Vector.h"

class Matrix3D
{
	public:
		Vector3D  m[3];
		Matrix3D(float i);
    	Matrix3D( const Vector3D& a, const Vector3D& b, const Vector3D& c );
		Matrix3D(float m00, float m10, float m20,
	  		float m01, float m11, float m21,
	  		float m02, float m12, float m22 );
		Matrix3D( const Matrix3D& m );
    	Vector3D& operator [] ( int i );
    	const Vector3D& operator [] ( int i ) const;
    	Matrix3D operator + ( const Matrix3D& m ) const;
		Matrix3D operator - ( const Matrix3D& m ) const;
		Matrix3D operator * ( const float s ) const;
		Matrix3D operator / ( const float s ) const;
		Matrix3D operator * ( const Matrix3D& m ) const;
		Matrix3D& operator += ( const Matrix3D& m );
	    Matrix3D& operator -= ( const Matrix3D& m );
	    Matrix3D& operator *= ( const float s );
    	Matrix3D& operator *= ( const Matrix3D& m );
    	Matrix3D& operator /= ( const float s );
    	Vector3D operator * ( const Vector3D& v ) const;
    	operator const float* () const;
		operator float* ();
		Matrix3D matrixCompMult( const Matrix3D& A, const Matrix3D& B );
		Matrix3D transpose( const Matrix3D& A );
};


#endif
