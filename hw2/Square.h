#ifndef SQUARE_H
#define SQUARE_H

#include "Shape.h"

class Square: public Shape
{
	public:
		void genPoints(GLfloat x, GLfloat y, GLfloat r); 
		Square(GLfloat x, GLfloat y, GLfloat r, GLenum mode);
};


#endif

