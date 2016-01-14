#include "Polygon.h"
#include "Line.h"
#include "Shape.h"
#include <iostream>
#include <algorithm>

using namespace std;

Polygon::Polygon()
{
}

void Polygon::addVertex(int x, int y)
{
	points.push_back(new Vector3D(x, y, 1.0));
}

bool isRightOf(Line edge, Vector3D* p)
{
	Vector3D *e1 = edge.points[0];
	Vector3D *e2 = edge.points[1];

	double crossProduct = ((e2->x - e1->x)*(p->y - e1->y) - (e2->y - e1->y)*(p->x - e1->x));
	return crossProduct <= 0;
}

void Polygon::tester(int minX, int minY, int maxX, int maxY)
{
	Line leftEdge = Line(minX, minY, minX, maxY);
	Line rightEdge = Line(maxX, maxY, maxX, minY);
	Line topEdge = Line(minX, maxY, maxX, maxY);
	Line bottomEdge = Line(maxX, minY, minX, minY);

	vector<Line> clippingEdges = { leftEdge, rightEdge, topEdge, bottomEdge };

	Vector3D *first = points[0];
	if (first->x > maxX || first->x < minX || first->y > maxY || first->y < minY)
	{
		points.erase(points.begin());
	}

	vector<Vector3D*> newPoints;

	for (Line e : clippingEdges)
	{
		for (int i = 0; i < points.size() - 1; i++)
		{
			Vector3D *p = points[i];
			Vector3D *q = points[i + 1];

			Line *lp = new Line(e.points[0]->x, e.points[0]->y, p->x, p->y);
			Line *lq = new Line(e.points[0]->x, e.points[0]->y, q->x, q->y);
			Line *pq = new Line(p->x, p->y, q->x, q->y);

			// case 1: inside inside
			if (!(isRightOf(e, p) && isRightOf(e, q)))
			{
				cout << "no" << endl;
			}
		}
	}
}


bool Polygon::clip(int minX, int minY, int maxX, int maxY)
{
	return SutherlandHodgman(minX, minY, maxX, maxY);
	//tester(minX, minY, maxX, maxY);
	//return true;
}

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

void Polygon::draw(char **screen, int height, int width)
{
	int index = 0;

	if (points.empty())
		return;

	while (index < points.size()-1)
	{
		Vector3D* p = points[index];
		Vector3D* q = points[index+1];

		Line* l = new Line(p->x, p->y, q->x, q->y);
		l->draw(screen, height, width);

		index++;
	}
}

bool xVals(Vector3D* first, Vector3D* second) 
{ 
	return (first->x < second->x); 
}

void Polygon::fill(char **screen, int height, int width)
{
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
}

bool Polygon::SutherlandHodgman(int minX, int minY, int maxX, int maxY)
{
	Line leftEdge = Line(minX, minY, minX, maxY);
	Line rightEdge = Line(maxX, maxY, maxX, minY);
	Line topEdge = Line(minX, maxY, maxX, maxY);
	Line bottomEdge = Line(maxX, minY, minX, minY);

	vector<Line> clippingEdges = { leftEdge, rightEdge, topEdge, bottomEdge }; 

	vector<Vector3D*> newPoints = vector<Vector3D*>(points);

	for (Line e : clippingEdges)
	{
		vector<Vector3D*>inputPoints = vector<Vector3D*>(newPoints);
		newPoints.clear();

		if (inputPoints.size() == 0)
			break;

		Vector3D *first = inputPoints[0];
		if (isRightOf(e, first))
			newPoints.push_back(first);

		for (int i = 0; i < inputPoints.size()-1; i++)
		{
			Vector3D *p = inputPoints[i];
			Vector3D *q = inputPoints[i + 1];

			Line *pq = new Line(p->x, p->y, q->x, q->y);

			// case 1: inside inside
			if (isRightOf(e, p) && isRightOf(e, q))
			{
				newPoints.push_back(q);
			}
			// case 2: inside outside
			else if (isRightOf(e, p) && !isRightOf(e, q))
			{
				Vector3D* intersect = intersection(e, *pq);
				if(NULL != intersect)
					newPoints.push_back(intersect);
			}
			// case 3: outside outside
			else if (!isRightOf(e, p) && !isRightOf(e, q))
			{
				// do nothing
			}
			// case 4: outside inside
			else if (!isRightOf(e, p) && isRightOf(e, q))
			{
				Vector3D* intersect = intersection(e, *pq);
				if(NULL != intersect)
					newPoints.push_back(intersect);
				newPoints.push_back(q);
			}
		}
	}

	points = vector<Vector3D*>(newPoints);

	return true;
}


