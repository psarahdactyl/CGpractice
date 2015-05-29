#include "Square.h"
#include <iostream>

using namespace std;

Square::Square(GLfloat x, GLfloat y, GLfloat r, GLenum mode, vec4 color): Shape(x, y, r, mode)
{
	this->numPoints = 4;
	this->mode = mode;
	this->genPoints(x, y, r);
	for(int i = 0; i < 4; i++)
	{
		Shape::colors.push_back( color );
	}
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
