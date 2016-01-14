#include "Transformation.h"
#include <iostream>
#include <cmath>

#include "Matrix.h"

using namespace std;

Vector3D Transformation::scale( Vector3D v, float scaleFactor )
{
	Matrix3D *m = new Matrix3D(scaleFactor, 0.0, 0.0,
	  		0.0, scaleFactor, 0.0,
	  		0.0, 0.0, 1.0);
	Vector3D ret = (*m)*v;
	return ret;

}

Vector3D Transformation::rotateCCW( Vector3D v, float radian )
{
	Matrix3D *m = new Matrix3D(cos(radian), sin(radian), 0.0,
	  		-sin(radian), cos(radian), 0.0,
	  		0.0, 0.0, 1.0);
	Vector3D ret = (*m)*v;
	return ret;
}

Vector3D Transformation::translate2D( Vector3D v, int translateX, int translateY )
{
	Matrix3D *m = new Matrix3D(1.0, 0.0, 0.0,
	  		0.0, 1.0, 0.0,
	  		translateX, translateY, 1.0);
	Vector3D ret = (*m)*v;
	return ret;
}

Matrix3D Transformation::createScaleMat( float scaleFactor )
{
	Matrix3D *m = new Matrix3D(scaleFactor, 0.0, 0.0,
	  		0.0, scaleFactor, 0.0,
	  		0.0, 0.0, 1.0);
	return *m;

}

Matrix3D Transformation::createRotateCCWMat( float radian )
{
	Matrix3D *m = new Matrix3D(cos(radian), sin(radian), 0.0,
	  		-sin(radian), cos(radian), 0.0,
	  		0.0, 0.0, 1.0);
	return *m;
}

Matrix3D Transformation::createTranslate2DMat( int translateX, int translateY )
{
	Matrix3D *m = new Matrix3D(1.0, 0.0, 0.0,
	  		0.0, 1.0, 0.0,
	  		translateX, translateY, 1.0);
	return *m;
}
