#include "Line.h"
#include <iostream>

using namespace std;

Line::Line(float x1, float y1, float x2, float y2)
{
	x1 = x1;
	y1 = y1;
	x2 = x2;
	y2 = y2;

	point1 = Vector3D(x1, y1, 1.0);
	point2 = Vector3D(x2, y2, 1.0);
}

