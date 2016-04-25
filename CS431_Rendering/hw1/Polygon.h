#ifndef POLYGON_H
#define POLYGON_H

#include "Vector4D.h"
#include <vector>

class Polygon
{
	public:
		Polygon();
		std::vector<Vector4D*> points;
		void addVertex(Vector4D* v);
		void addVertex(int x, int y, int z);
};


#endif

