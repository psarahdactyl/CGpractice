#ifndef MATRIX4D_H
#define MATRIX4D_H

#include "Vector4D.h"

class Matrix4D
{
	public:
		Vector4D  m[4];
		Matrix4D(float i);
    	Matrix4D( const Vector4D& a, const Vector4D& b, const Vector4D& c, const Vector4D& d  );
		Matrix4D(float m00, float m10, float m20, float m30,
	  			float m01, float m11, float m21, float m31,
	  			float m02, float m12, float m22, float m32,
	  			float m03, float m13, float m23, float m33);
		Matrix4D( const Matrix4D& m );
    	Vector4D& operator [] ( int i );
    	const Vector4D& operator [] ( int i ) const;
    	Matrix4D operator + ( const Matrix4D& m ) const;
		Matrix4D operator - ( const Matrix4D& m ) const;
		Matrix4D operator * ( const float s ) const;
		Matrix4D operator / ( const float s ) const;
		Matrix4D operator * ( const Matrix4D& m ) const;
		Matrix4D& operator += ( const Matrix4D& m );
	    Matrix4D& operator -= ( const Matrix4D& m );
	    Matrix4D& operator *= ( const float s );
    	Matrix4D& operator *= ( const Matrix4D& m );
    	Matrix4D& operator /= ( const float s );
    	Vector4D operator * ( const Vector4D& v ) const;
    	operator const float* () const;
		operator float* ();
		Matrix4D matrixCompMult( const Matrix4D& A, const Matrix4D& B );
		Matrix4D transpose( const Matrix4D& A );
};


#endif
