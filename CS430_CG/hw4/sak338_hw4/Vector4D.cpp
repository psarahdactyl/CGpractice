#include "Vector4D.h"

#include <iostream>
#include <cmath>

using namespace std;

    Vector4D::Vector4D()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
        this->w = 0.0;
    }

    Vector4D::Vector4D( float x, float y, float z, float w )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    float& Vector4D::operator [] ( int i ) 
    { 
        return *(&x + i); 
    }

    const float Vector4D::operator [] ( int i ) const 
    { 
        return *(&x + i); 
    }

    Vector4D Vector4D::operator - () const  
	{ 
        return Vector4D( -x, -y, -z, -w ); 
    }

    Vector4D Vector4D::operator + ( const Vector4D& v ) const
	{ 
        return Vector4D( x + v.x, y + v.y, z + v.z, w + v.w ); 
    }

    Vector4D Vector4D::operator - ( const Vector4D& v ) const
	{ 
        return Vector4D( x - v.x, y - v.y, z - v.z, w - v.w ); 
    }

    Vector4D Vector4D::operator * ( const float s ) const
	{ 
        return Vector4D( s*x, s*y, s*z, s*w ); 
    }

    Vector4D Vector4D::operator * ( const Vector4D& v ) const
	{ 
        return Vector4D( x*v.x, y*v.y, z*v.z, w*v.w ); 
    }

    Vector4D operator * ( const float s, const Vector4D& v )
    { 
        return v * s; 
    }

    Vector4D Vector4D::operator / ( const float s ) const 
    {
        if(s != 0.0)
        {
            float r = 1.0 / s;
            return *this * r;
        }
    }

    Vector4D& Vector4D::operator += ( const Vector4D& v )
	{ 
        x += v.x;  
        y += v.y;  
        z += v.z; 
	   	w += v.w;	
        return *this; 
    }

    Vector4D& Vector4D::operator -= ( const Vector4D& v )
	{ 
        x -= v.x;  
        y -= v.y;  
        z -= v.z; 
	   	w -= v.w;	
        return *this; 
    }

    Vector4D& Vector4D::operator *= ( const float s )
	{ 
        x *= s;  
        y *= s;  
        z *= s;
	  	w *= s;	
        return *this; 
    }

    Vector4D& Vector4D::operator *= ( const Vector4D& v )
	{ 
        x *= v.x;  
        y *= v.y;  
        z *= v.z; 
	   	w *= v.w;	
        return *this; 
    }

    Vector4D& Vector4D::operator /= ( const float s ) 
    {

	   float r = 1.0 / s;
	   *this *= r;

	   return *this;
    }

    Vector4D::operator const float* () const
	{ 
        return static_cast<const float*>( &x ); 
    }

    Vector4D::operator float* ()
	{ 
        return static_cast<float*>( &x ); 
    }

    float Vector4D::dot( const Vector4D& u, const Vector4D& v ) 
    {
        return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w ;
    }

    float Vector4D::length( const Vector4D& v ) 
    {
        return sqrt( dot(v,v) );
    }

    Vector4D Vector4D::normalize( const Vector4D& v ) 
    {
        return v / length(v);
    }

