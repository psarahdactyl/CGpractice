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
		}
		//points[i]->z = 1;

	}
}


void Polygon::draw(float **zBuff, char **screen, int height, int width, float zFront)
{
	int index = 0;

	if (points.empty())
		return;

	while (index < points.size()-1)
	{
		Vector4D* p = points[index];
		Vector4D* q = points[index+1];

		Line* l = new Line(p->x, p->y, p->z, q->x, q->y,q->z);
		l->zVals.push_back(zVals[0]);
		l->zVals.push_back(zVals[1]);
		l->draw(zBuff, screen, height, width, zFront);

		index++;
	}
	Vector4D* p = points[points.size()-1];
	Vector4D* q = points[0];

	Line* l = new Line(p->x, p->y, p->z, q->x, q->y,q->z);
	l->draw(zBuff, screen, height, width, zFront);
}

bool xVals(Vector4D* first, Vector4D* second) 
{ 
	return (first->x < second->x); 
}

/*
Vector3D* intersection(Line first, Line second)
{
	float t0 = 0;
	float t2 = 0;

	Vector3D *P0 = first.points[0];
	Vector3D *P1 = first.points[1];
	Vector3D *P2 = second.points[0];
	Vector3D *P3 = second.points[1];

	int x0 = P0->x;
	int y0 = P0->y;
	int x2 = P2->x;
	int y2 = P2->y;

	//cout << "x0 " << x0 << endl;
	//cout << "y0 " << y0 << endl;
	//cout << "x2 " << x2 << endl;
	//cout << "y2 " << y2 << endl;

	int Dx0 = P1->x - P0->x;
	int Dy0 = P1->y - P0->y;
	int Dx2 = P3->x - P2->x;
	int Dy2 = P3->y - P2->y;

	//cout << "Dx0 " << Dx0 << endl;
	//cout << "Dy0 " << Dy0 << endl;
	//cout << "Dx2 " << Dx2 << endl;
	//cout << "Dy2 " << Dy2 << endl;

	double ix;
	double iy;

	if ((Dy0*Dx2 - Dx0*Dy2) == 0 || (Dy2*Dx0 - Dx2*Dy0) == 0)
	{
		return NULL;
	}
	else 
	{	
		float N = (x0-x2)*Dy2 + (y2-y0)*Dx2;
		float D = Dy0*Dx2 - Dx0*Dy2;

		float M = (x2-x0)*Dy0 + (y0-y2)*Dx0;
		float P = Dy2*Dx0 - Dx2*Dy0;
	   	//cout << N << endl;	
		//cout << D << endl;
		//cout << N / D << endl;

		t0 = N / D;
		t2 = M / P;
	
		if (t0 < 0 || t2 < 0 || t0 > 1 || t2 > 1)
			return NULL;

		ix = x0 + t0*(P1->x - x0);
		iy = y0 + t0*(P1->y - y0);

		//cout << ix << endl;
		//cout << iy << endl;
	}
	return new Vector3D(ix, iy, 1.0);
}
*/

void Polygon::fill(float ** zBuff, char **screen, int height, int width)
{
	/*
	if (points.empty())
		return;

	for (int i = 0; i < height; i++)
	{
		vector<Vector3D*> scanLineExtrema;
		for(int index = 0; index < points.size(); index++)
		{
			Vector3D* p = points[index];
			Vector3D* q;
			if(index == points.size()-1)
				q = points[0];
			else
				q = points[index + 1];

			int ymin = min(p->y, q->y);
			int ymax = max(p->y, q->y);
			if (p->y == q->y)
				continue; // skip horizontals
			if (i == (int)ymax)
				continue; // skip top vertex

			if (ymin <= i && i < ymax)
			{
				Line* l = new Line((int)p->x, (int)p->y, (int)q->x, (int)q->y);
				Line* scanline = new Line(0, i, width, i);
				Vector3D* intersect = intersection(*l, *scanline);
				scanLineExtrema.push_back(intersect);
			}
		}

		sort(scanLineExtrema.begin(), scanLineExtrema.end(), xVals);

		int parityBit = 0;
		for (int j = 0; j < width; j++)
		{
			for(Vector3D* p : scanLineExtrema)
			{
				if (j == (int)p->x)
					parityBit = !parityBit;
				if (parityBit)
					screen[i][j] = '-';
			}
		}
	}
	*/
}

