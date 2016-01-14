#include "Transformation.h"
#include <iostream>
#include <cmath>

#include "Matrix3D.h"

using namespace std;

Vector3D Transformation::scale( Vector3D v, float scaleFactorX, float scaleFactorY )
{
	Matrix3D *m = new Matrix3D(scaleFactorX, 0.0, 0.0,
	  		0.0, scaleFactorY, 0.0,
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

Vector4D Transformation::translate3D( Vector4D v, int translateX, int translateY, int translateZ )
{
	Matrix4D *m = new Matrix4D(1,0,0,translateX,
	  						   0,1,0,translateY,
	  						   0,0,1,translateZ,
							   0,0,0,1);
	Vector4D ret = (*m)*v;
	return ret;
}
Matrix3D Transformation::createScaleMat( float scaleFactorX, float scaleFactorY )
{
	Matrix3D *m = new Matrix3D(scaleFactorX, 0.0, 0.0,
	  		0.0, scaleFactorY, 0.0,
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

