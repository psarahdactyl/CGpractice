#include "Triangle.h"
#include <iostream>

using namespace std;

Triangle::Triangle(GLfloat x, GLfloat y, GLfloat r, GLenum mode): Shape(x, y, r, mode)
{
	this->numPoints = 3;
	this->mode = mode;
	this->genPoints(x, y, r);
}

void Triangle::genPoints(GLfloat x, GLfloat y, GLfloat r) 
{
	float unit = (2 * M_PI);
	float inc = unit / numPoints; // get the number to increment by
	for(float i = (M_PI / 2) ; i <= unit; i+=inc)
	{
		float X = x + (r*cos(i));
		float Y = y + (r*sin(i));
		Shape::coords.push_back( vec2(X, Y) );
	}
}	
