#include "Vector3D.h"

#include <iostream>
#include <cmath>

using namespace std;

    Vector3D::Vector3D()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }

    Vector3D::Vector3D( float x, float y, float z )
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float& Vector3D::operator [] ( int i ) 
    { 
        return *(&x + i); 
    }

    const float Vector3D::operator [] ( int i ) const 
    { 
        return *(&x + i); 
    }

    Vector3D Vector3D::operator - () const  
	{ 
        return Vector3D( -x, -y, -z ); 
    }

    Vector3D Vector3D::operator + ( const Vector3D& v ) const
	{ 
        return Vector3D( x + v.x, y + v.y, z + v.z ); 
    }

    Vector3D Vector3D::operator - ( const Vector3D& v ) const
	{ 
        return Vector3D( x - v.x, y - v.y, z - v.z ); 
    }

    Vector3D Vector3D::operator * ( const float s ) const
	{ 
        return Vector3D( s*x, s*y, s*z ); 
    }

    Vector3D Vector3D::operator * ( const Vector3D& v ) const
	{ 
        return Vector3D( x*v.x, y*v.y, z*v.z ); 
    }

    Vector3D operator * ( const float s, const Vector3D& v )
    { 
        return v * s; 
    }

    Vector3D Vector3D::operator / ( const float s ) const 
    {
        if(s != 0.0)
        {
            float r = 1.0 / s;
            return *this * r;
        }
    }

    Vector3D& Vector3D::operator += ( const Vector3D& v )
	{ 
        x += v.x;  
        y += v.y;  
        z += v.z;  
        return *this; 
    }

    Vector3D& Vector3D::operator -= ( const Vector3D& v )
	{ 
        x -= v.x;  
        y -= v.y;  
        z -= v.z;  
        return *this; 
    }

    Vector3D& Vector3D::operator *= ( const float s )
	{ 
        x *= s;  
        y *= s;  
        z *= s;  
        return *this; 
    }

    Vector3D& Vector3D::operator *= ( const Vector3D& v )
	{ 
        x *= v.x;  
        y *= v.y;  
        z *= v.z;  
        return *this; 
    }

    Vector3D& Vector3D::operator /= ( const float s ) 
    {

	   float r = 1.0 / s;
	   *this *= r;

	   return *this;
    }

    Vector3D::operator const float* () const
	{ 
        return static_cast<const float*>( &x ); 
    }

    Vector3D::operator float* ()
	{ 
        return static_cast<float*>( &x ); 
    }

    float Vector3D::dot( const Vector3D& u, const Vector3D& v ) 
    {
        return u.x*v.x + u.y*v.y + u.z*v.z ;
    }

    float Vector3D::length( const Vector3D& v ) 
    {
        return sqrt( dot(v,v) );
    }

    void Vector3D::normalize() 
    {
		if(length(*this) == 0)
			return;

        Vector3D c = *this / length(*this);
	    this->x = c.x;
	    this->y = c.y;
	    this->z = c.z;
    }

    Vector3D* Vector3D::cross(const Vector3D& b)
    {
        return new Vector3D( y * b.z - z * b.y,
						     z * b.x - x * b.z,
							 x * b.y - y * b.x );
    }




