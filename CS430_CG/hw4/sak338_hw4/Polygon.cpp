#include "Polygon.h"
#include "Line.h"
#include "Shape.h"
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

void Polygon::addVertex(int x, int y, int z)
{
	addVertex(new Vector4D(x, y, z, 1.0));
}

bool isRightOf(Line edge, Vector3D* p)
{
	Vector4D *e1 = edge.points[0];
	Vector4D *e2 = edge.points[1];

	double crossProduct = ((e2->x - e1->x)*(p->y - e1->y) - (e2->y - e1->y)*(p->x - e1->x));
	return crossProduct <= 0;
}

bool Polygon::clip(float PRPz, float front, float back, bool perspective)
{
	for (int i = 0; i < points.size()-1; i++)
	{
		Line* l = new Line(points[i]->x, points[i]->y, points[i]->z, points[i + 1]->x, points[i + 1]->y, points[i + 1]->z);
		bool inside = l->clip(PRPz, front, back, perspective);
		if (!inside)
			return false;
	}

	return true;
}

void Polygon::convert(Matrix4D view, Matrix4D projection)
{
	for (int i = 0; i < points.size(); i++)
	{
		Matrix4D VP = projection * view;
		Vector4D p = VP * *points[i];
		points[i]->x = p.x;
		points[i]->y = p.y;
		points[i]->z = p.z;
	}
}

void Polygon::project(bool perspective)
{
	for (int i = 0; i < points.size(); i++)
	{
		if (perspective)
		{
			points[i]->x = points[i]->x / (-1*points[i]->z);
			points[i]->y = points[i]->y / (-1*points[i]->z);
			points[i]->z = 1;
		}
		else
			points[i]->z = 1;

	}
}


void Polygon::draw(char **screen, int height, int width)
{
	int index = 0;

	if (points.empty())
		return;

	while (index < points.size()-1)
	{
		Vector4D* p = points[index];
		Vector4D* q = points[index+1];

		Line* l = new Line(p->x, p->y, p->z, q->x, q->y,q->z);
		l->draw(screen, height, width);

		index++;
	}
	Vector4D* p = points[points.size()-1];
	Vector4D* q = points[0];

	Line* l = new Line(p->x, p->y, p->z, q->x, q->y,q->z);
	l->draw(screen, height, width);
}

bool xVals(Vector4D* first, Vector4D* second) 
{ 
	return (first->x < second->x); 
}



