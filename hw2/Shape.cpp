#include "Shape.h"

vector<vec2> Shape::coords;

Shape::Shape(GLfloat x, GLfloat y, GLfloat r, GLenum mode) 
{
	this->mode = mode;
}


