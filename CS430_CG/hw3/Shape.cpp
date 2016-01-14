#include "Shape.h"
#include "Matrix.h"
#include "Transformation.h"
#include <iostream>

void Shape::transform(float scaleFactorX, float scaleFactorY, float radians, int translateX, int translateY)
{
	//scale
	Matrix3D s = Transformation::createScaleMat(scaleFactorX, scaleFactorY);
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

void Shape::worldToViewport(int xmin, int xmax, int ymin, int ymax, 
							int umin, int umax, int vmin, int vmax)
{
	float dx = xmax - xmin;
	float dy = ymax - ymin;
	float du = umax - umin;
	float dv = vmax - vmin;

	// viewport scale
	Matrix3D s = Transformation::createScaleMat(du/dx, dv/dy);
	// world translate
	Matrix3D wt = Transformation::createTranslate2DMat(-1*xmin, -1*ymin);
	// screen translate
	Matrix3D st = Transformation::createTranslate2DMat(umin, vmin);

	Matrix3D m = st*s*wt; 

	for (Vector3D* p : points)
	{
		Vector3D n = m * (*p);
		p->x = n.x;
		p->y = n.y;
		Vector3D g = Transformation::translate2D(*p, 0, -501);
		p->x = g.x;
		p->y = g.y;
		p->y = -1 * p->y;
	}
}
