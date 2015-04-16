#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <vector>
#include "Angel.h"
#include "math.h"

using namespace std;

class Shape 
{
	public:
		int numPoints;
		GLenum mode;
		static vector<vec2> coords;
		static vector<vec4> colors;
		Shape(GLfloat x, GLfloat y, GLfloat r, GLenum mode); 
};

#endif

