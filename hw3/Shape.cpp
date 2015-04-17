#include "Shape.h"

vector<vec2> Shape::coords;
vector<vec4> Shape::colors;

Shape::Shape(GLfloat x, GLfloat y, GLfloat r, GLenum mode) 
{
	this->mode = mode;
}


