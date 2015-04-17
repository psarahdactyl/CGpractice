#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <vector>
#include "Angel.h"
#include "math.h"

using namespace std;

class Shape 
{
	protected:
		int numPoints;
		GLenum mode;
		GLuint vao, buffer, program, location; 
		//vec2 [] coords;
	public:
		static vector<vec2> coords;
		void setup();
		void draw();
		Shape(GLfloat x, GLfloat y, GLfloat r, int n, GLenum mode, GLuint program); 
};

#endif

