#ifndef POLYGON_H
#define POLYGON_H

#include "Vector3D.h"
#include "Vector4D.h"
#include <vector>

class Polygon
{
	public:
		Polygon();
		std::vector<Vector4D*> points;
        std::vector<Vector3D*> normals;
		void addVertex(Vector4D* v);
		void addVertex(int x, int y, int z);
		void addNormal(Vector3D* v);
		void addNormal(int x, int y, int z);
};


#endif

