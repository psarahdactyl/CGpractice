#include "Circle.h"
#include <iostream>

using namespace std;

Circle::Circle(GLfloat x, GLfloat y, GLfloat r, GLenum mode): Shape(x, y, r, mode)
{
	this->numPoints = 1000;
	this->mode = mode;
	this->scaleX = 1.0f;
	this->scaleY = 1.0f;
	this->genPoints(x, y, r);
	for(int i = 0; i < 1000; i++)
	{
		Shape::colors.push_back( vec4(1.0, 0.0, 0.0, 1.0) );
	}
}

Circle::Circle(GLfloat x, GLfloat y, float scaleX, float scaleY, GLfloat r, GLenum mode) : Shape(x, y, r, mode)
{
	this->numPoints = 1000;
	this->mode = mode;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->genPoints(x, y, r);
	for(float i = 1.0; i > 0.0; i--)
	{
		Shape::colors.push_back( vec4(i, 0.0, 0.0, 1.0) );
	}
}

void Circle::genPoints(GLfloat x, GLfloat y, GLfloat r)
{
	float unit = (2 * M_PI);
	float inc = unit / numPoints; // get the number to increment by
	for(float i = 0; i <= unit; i+=inc)
	{
		float X = x + (r * cos(i) * scaleX);
		float Y = y + (r * sin(i) * scaleY);
		Shape::coords.push_back( vec2(X, Y) );
	}
	
}	
