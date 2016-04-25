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
        return u.x*v.x + u.y*v.y + u.z*v.z ; //# fuck w
    }

    float Vector4D::length( const Vector4D& v ) 
    {
        return sqrt( dot(v,v) );
    }

    Vector4D Vector4D::normalize( const Vector4D& v ) 
    {
        return v / length(v);
    }

    void Vector4D::normalize()
    {
        if(length(*this) == 0)
         return;

        Vector4D c = *this / length(*this);
        this->x = c.x;
        this->y = c.y;
        this->z = c.z;
        this->w = 0;
}

    Vector4D* Vector4D::cross(const Vector4D& b)
    {
        return new Vector4D( y * b.z - z * b.y,
                         z * b.x - x * b.z,
                         x * b.y - y * b.x,
                        1);
    }

    Vector3D* cross(const Vector4D& a, const Vector4D& b )
    {
        Vector3D* ret = new Vector3D();
        ret->x = a.y * b.z - a.z * b.y;
		ret->y = a.z * b.x - a.x * b.z;
		ret->z = a.x * b.y - a.y * b.x;
        return ret;
    }
