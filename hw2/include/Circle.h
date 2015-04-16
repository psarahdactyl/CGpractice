#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle: public Shape
{
	public:
		void genPoints(GLfloat x, GLfloat y, GLfloat r); 
		Circle(GLfloat x, GLfloat y, GLfloat r, GLenum mode);
};


#endif

