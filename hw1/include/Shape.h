#ifndef SHAPE_H
#define SHAPE_H

#include "Angel.h"

class Shape 
{
	private:
		int numPoints;
		GLuint buffer, vao;
		GLenum mode;
	public:
		void draw();
		Shape(vec2 *verts, int numPoints, GLenum mode, GLuint program); 
};

#endif


