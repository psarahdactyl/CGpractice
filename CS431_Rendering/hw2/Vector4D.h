#ifndef VECTOR4D_H
#define VECTOR4D_H

#include "Vector3D.h"

class Vector4D
{
	public:
		float x;
		float y;
		float z;
		float w;
		Vector4D(float x, float y, float z, float w);
		Vector4D();
    	float& operator [] ( int i );
    	const float operator [] ( int i ) const;
    	Vector4D operator - () const;
    	Vector4D operator + ( const Vector4D& v ) const;
    	Vector4D operator - ( const Vector4D& v ) const;
    	Vector4D operator * ( const float s ) const;
    	Vector4D operator * ( const Vector4D& v ) const;
    	friend Vector4D operator * ( const float s, const Vector4D& v );
   		Vector4D operator / ( const float s ) const;
    	Vector4D& operator += ( const Vector4D& v );
    	Vector4D& operator -= ( const Vector4D& v );
    	Vector4D& operator *= ( const float s );
    	Vector4D& operator *= ( const Vector4D& v );
		Vector4D& operator /= ( const float s );
    	operator const float* () const;
    	operator float* ();
    	float dot( const Vector4D& u, const Vector4D& v );
    	float length( const Vector4D& v );
    	Vector4D normalize( const Vector4D& v );
        void normalize();
		Vector4D* cross(const Vector4D& b);
};


Vector3D* cross(const Vector4D& a, const Vector4D& b );

#endif

