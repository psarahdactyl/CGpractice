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
		zVals.push_back(points[i]->z);
		points[i]->z = 1;

	}
}


void Polygon::draw(float **zBuff, char **screen, int height, int width)
{
	int index = 0;

	if (points.empty())
		return;

	while (index < points.size()-1)
	{
		Vector4D* p = points[index];
		Vector4D* q = points[index+1];

		Line* l = new Line(p->x, p->y, p->z, q->x, q->y,q->z);
		l->zVals.push_back(zVals[index]);
		l->zVals.push_back(zVals[index+1]);
		l->baseColor = baseColor;
		l->draw(zBuff, screen, height, width);

		index++;
	}
	Vector4D* p = points[points.size()-1];
	Vector4D* q = points[0];

	Line* l = new Line(p->x, p->y, p->z, q->x, q->y,q->z);
	l->zVals.push_back(zVals[points.size() - 1]);
	l->zVals.push_back(zVals[0]);
	l->baseColor = baseColor;
	l->draw(zBuff, screen, height, width);
}

bool xVals(Vector4D* first, Vector4D* second) 
{ 
	return (first->x < second->x); 
}

Vector4D* intersection(Line first, Line second)
{
	float t0 = 0;
	float t2 = 0;

	Vector4D *P0 = first.points[0];
	Vector4D *P1 = first.points[1];
	Vector4D *P2 = second.points[0];
	Vector4D *P3 = second.points[1];

	int x0 = P0->x;
	int y0 = P0->y;
	int x2 = P2->x;
	int y2 = P2->y;

	int Dx0 = P1->x - P0->x;
	int Dy0 = P1->y - P0->y;
	int Dx2 = P3->x - P2->x;
	int Dy2 = P3->y - P2->y;

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
		t0 = N / D;
		t2 = M / P;
	
		if (t0 < 0 || t2 < 0 || t0 > 1 || t2 > 1)
			return NULL;

		ix = x0 + t0*(P1->x - x0);
		iy = y0 + t0*(P1->y - y0);

	}
	return new Vector4D(ix, iy, 1.0, 1.0);
}

void Polygon::fill(float ** zBuff, char **screen, int height, int width, float zMin, float zMax)
{
	if (points.empty())
		return;

	for (int i = 0; i < height; i++)
	{
		vector<Vector4D*> scanLineExtrema;
		for(int index = 0; index < points.size(); index++)
		{
			Vector4D* p = points[index];
			Vector4D* q;
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

			float z1, z2, y1, y2;
			if (p->y > q->y)
			{
				y1 = p->y;
				z1 = zVals.at(index);
				y2 = q->y;
				if(index == points.size()-1)
					z2 = zVals.at(0);
				else
					z2 = zVals.at(index+1);
			}
			else
			{
				y1 = q->y;
				if(index == points.size()-1)
					z1 = zVals.at(0);
				else
					z1 = zVals.at(index+1);
				y2 = p->y;
				z2 = zVals.at(index);
			}

			if (ymin <= i && i < ymax)
			{
				Line* l = new Line((int)p->x, (int)p->y, 1, (int)q->x, (int)q->y, 1);
				float za = z1 - (z1 - z2) * (y1 - i) / (y1 - y2);
				Line* scanline = new Line(0, i, 1, width, i, 1);
				Vector4D* intersect = intersection(*l, *scanline);
				if(intersect != NULL)
					intersect->z = za;
				scanLineExtrema.push_back(intersect);
			}
		}

		int x = scanLineExtrema.size();

		sort(scanLineExtrema.begin(), scanLineExtrema.end(), xVals);

		int parityBit = 0;
		Vector4D* prevEx = NULL;
		Vector4D* nextEx = NULL;
		for (int j = 0; j < width; j++)
		{
			for(Vector4D* p : scanLineExtrema)
			{
				if (j == (int)p->x)
				{
					parityBit = !parityBit;
					prevEx = p;
					nextEx = NULL;
				}
			}


			if (parityBit)
			{
				if (nextEx == NULL)
				{
					for (int k = j + 1; k < width; k++)
					{
						for (Vector4D* q : scanLineExtrema)
						{
							if (k == (int)q->x)
								nextEx = q;
						}
					}
				}

				float za = prevEx->z;
				float zb = nextEx->z;
				float xa = prevEx->x;
				float xb = nextEx->x;
				float zp = zb - (zb - za) * (xb - (float)j) / (xb - xa);
				zMin = -1;
				if (zp < zMax && zp > zBuff[j][i])
				{
					zBuff[j][i] = zp;
					float shade = 19.0 * (zp - zMin) / (zMax - zMin);
					screen[i][j] = (char)(baseColor + 19 - shade);
				}
			}
		}
	}
}

