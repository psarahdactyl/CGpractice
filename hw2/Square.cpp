#include "Square.h"
#include <iostream>

using namespace std;

Square::Square(GLfloat x, GLfloat y, GLfloat r, GLuint n, GLenum mode): Shape(x, y, r, n, mode)
{
	this->numPoints = 4;
	this->mode = mode;
	this->genPoints(x, y, r);
}

void Square::genPoints(GLfloat x, GLfloat y, GLfloat r) 
{
	float unit = (2 * M_PI);
	float inc = unit / numPoints; // get the number to increment by
	for(float i = (M_PI / 4) ; i <= unit; i+=inc)
	{
		float X = x + (r*cos(i));
		float Y = y + (r*sin(i));
		Shape::coords.push_back( vec2(X, Y) );
	}
}	
