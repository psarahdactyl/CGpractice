#include "Circle.h"
#include <iostream>

using namespace std;

Circle::Circle(GLfloat x, GLfloat y, GLfloat r, GLenum mode): Shape(x, y, r, mode)
{
	this->numPoints = 1000;
	this->mode = mode;
	this->genPoints(x, y, r, n);
}

void Circle::genPoints(GLfloat x, GLfloat y, GLfloat r)
{
	float unit = (2 * M_PI);
	cout << unit << endl;
	float inc = unit / numPoints; // get the number to increment by
	cout << inc << endl;
	for(float i = 0; i <= unit; i+=inc)
	{
		float X = x + (r*cos(i));
		float Y = y + (r*sin(i));
		Shape::coords.push_back( vec2(X, Y) );
	}
	
}	
