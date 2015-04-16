#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle: public Shape
{
	public:
		void genPoints(GLfloat x, GLfloat y, GLfloat r); 
		Circle(GLfloat x, GLfloat y, GLfloat r, GLenum mode);
		Circle(GLfloat x, GLfloat y, float scaleX, float scaleY, GLfloat r, GLenum mode);
	private:
		float scaleX, scaleY;
};


#endif

