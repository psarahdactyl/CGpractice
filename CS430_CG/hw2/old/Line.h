#ifndef LINE_H
#define LINE_H

#include "Vector.h"

class Line
{
	public:
		Vector3D point1;
		Vector3D point2;
		float x1;
		float y1;
		float x2;
		float y2;
		Line(float x1, float y1, float x2, float y2);
};


#endif

