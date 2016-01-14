#include "Shape.h"
#include "Matrix.h"
#include "Transformation.h"
#include <iostream>

void Shape::transform(float scaleFactor, float radians, int translateX, int translateY)
{
	//scale
	Matrix3D s = Transformation::createScaleMat(scaleFactor);
	//rotate
	Matrix3D r = Transformation::createRotateCCWMat(radians);
	//translate
	Matrix3D t = Transformation::createTranslate2DMat(translateX, translateY);

	Matrix3D m = t*r*s;

	for (Vector3D* p : points)
	{
		Vector3D n = m * (*p);
		p->x = n.x;
		p->y = n.y;
	}
}

void Shape::worldToScreen(int lowerBoundX, int lowerBoundY, int upperBoundY)
{
	int screenHeight = upperBoundY - lowerBoundY;

	for(Vector3D* p : points)
	{
		p->x = p->x - lowerBoundX;
		p->y = p->y - lowerBoundY;
		Vector3D n = Transformation::translate2D(*p, 0, -1 * screenHeight);
		p->x = n.x;
		p->y = n.y;
		p->y = -1 * p->y;
	}
}
