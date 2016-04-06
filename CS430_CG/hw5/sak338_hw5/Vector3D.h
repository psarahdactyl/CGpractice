#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D
{
	public:
		float x;
		float y;
		float z;
		Vector3D(float x, float y, float z);
		Vector3D();
    	float& operator [] ( int i );
    	const float operator [] ( int i ) const;
    	Vector3D operator - () const;
    	Vector3D operator + ( const Vector3D& v ) const;
    	Vector3D operator - ( const Vector3D& v ) const;
    	Vector3D operator * ( const float s ) const;
    	Vector3D operator * ( const Vector3D& v ) const;
    	friend Vector3D operator * ( const float s, const Vector3D& v );
   		Vector3D operator / ( const float s ) const;
    	Vector3D& operator += ( const Vector3D& v );
    	Vector3D& operator -= ( const Vector3D& v );
    	Vector3D& operator *= ( const float s );
    	Vector3D& operator *= ( const Vector3D& v );
		Vector3D& operator /= ( const float s );
    	operator const float* () const;
    	operator float* ();
    	float dot( const Vector3D& u, const Vector3D& v );
    	float length( const Vector3D& v );
    	void normalize();
    	Vector3D* cross(const Vector3D& b);
};


#endif

