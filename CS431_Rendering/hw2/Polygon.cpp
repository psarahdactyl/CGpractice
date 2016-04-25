#include "Polygon.h"
#include <iostream>
#include <algorithm>

using namespace std;

Polygon::Polygon()
{
}

void Polygon::addVertex(Vector4D* v)
{
	points.push_back(v);
}

void Polygon::addNormal(Vector3D* v)
{
	normals.push_back(v);
}

void Polygon::addVertex(int x, int y, int z)
{
	addVertex(new Vector4D(x, y, z, 1.0));
}

