#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Vector.h"
#include "Matrix.h"

class Transformation
{
	public:
		static Vector3D scale( Vector3D v, float scaleFactorX, float scaleFactorY );
		static Vector3D rotateCCW( Vector3D v, float radian );
		static Vector3D translate2D( Vector3D v, int translateX, int translateY );
		static Matrix3D createScaleMat( float scaleFactorX, float scaleFactorY );
		static Matrix3D createRotateCCWMat( float radian );
		static Matrix3D createTranslate2DMat( int translateX, int translateY );
};


#endif
