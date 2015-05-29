#include "Triangle.h"
#include <iostream>

using namespace std;

Triangle::Triangle(GLfloat x, GLfloat y, GLfloat r, GLenum mode, vec4 color): Shape(x, y, r, mode)
{
	this->numPoints = 3;
	this->mode = mode;
	this->genPoints(x, y, r);
	for(int i = 0; i < 3; i++)
	{
		Shape::colors.push_back( color );
	}

	/*
	Shape::colors.push_back( vec4(1.0, 0.0, 0.0, 1.0) );
	Shape::colors.push_back( vec4(0.0, 1.0, 0.0, 1.0) );
	Shape::colors.push_back( vec4(0.0, 0.0, 1.0, 1.0) );
	*/
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
