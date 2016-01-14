#ifndef LINE_H
#define LINE_H

#include "Vector.h"
#include "Shape.h"

const int inside = 0; 	// 0000
const int outLeft = 1;	// 0001
const int outRight = 2;	// 0010
const int outBottom = 4;	// 0100
const int outTop = 8;		// 1000

class Line : public Shape
{
	public:
		Line(float x1, float y1, float x2, float y2);
		virtual bool clip(int minX, int minY, int maxX, int maxY);
		virtual void draw(char ** screen);
	private:
		bool CohenSutherland(int minX, int minY, int maxX, int maxY);
};


#endif

