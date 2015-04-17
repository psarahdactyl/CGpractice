#include "Circle.h"
#include <iostream>

using namespace std;

Circle::Circle(GLfloat x, GLfloat y, GLfloat r, int n, GLenum mode): Shape(x, y, r, n, mode)
{
	this->mode = mode;
	this->genPoints(x, y, r, n);
}

void Circle::genPoints(GLfloat x, GLfloat y, GLfloat r, int n){
	numPoints = n;
	float unit = (2 * M_PI) / 100;
	cout << unit << endl;
	float inc = unit / n; // get the number to increment by
	cout << inc << endl;
	for(float i = 0; i < unit; i+=inc)
	{
		cout << (x + r*cos(i))/100 << "    " <<  (y + r*sin(i))/100 << endl;
		Shape::coords.push_back(vec2((x + r*cos(i))/100, (y + r*sin(i))/100));
	}
	
}	
