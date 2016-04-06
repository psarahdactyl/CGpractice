#ifndef LINE_H
#define LINE_H

//#include "Vector3D.h"
#include "Vector4D.h"
#include "Shape.h"

const int inside = 0; 	// 000000
const int inFront = 1;	// 000001
const int behind = 2;	// 000010
const int leftOf = 4;	// 000100
const int rightOf = 8;		// 001000
const int below = 16;		// 010000
const int above = 32;		// 100000

class Line : public Shape
{
	public:
		Line(float x1, float y1, float z1, float x2, float y2, float z2);
		virtual bool clip(float prpZ, float front, float back, bool perspective);
		virtual void draw(float ** zBuff, char ** screen, int height, int width);
		virtual void fill(float ** zBuff, char ** screen, int height, int width, float zMin, float zMax);
	private:
		bool CohenSutherland(float PRPz, float front, float back, bool perspective);
};


#endif

