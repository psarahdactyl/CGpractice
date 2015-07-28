#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle: public Shape
{
	public:
		void genPoints(GLfloat x, GLfloat y, GLfloat r); 
		Triangle(GLfloat x, GLfloat y, GLfloat r, GLenum mode, vec4 color);
};


#endif

