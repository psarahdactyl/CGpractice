#include "Line.h"
#include "Shape.h"
#include <iostream>
#include <cmath>

using namespace std;

Line::Line(float x1, float y1, float x2, float y2)
{
	Vector3D* point1 = new Vector3D(x1, y1, 1.0);
	Vector3D* point2 = new Vector3D(x2, y2, 1.0);
	points.push_back(point1);
	points.push_back(point2);
}

bool Line::clip(int minX, int minY, int maxX, int maxY) 
{
	return CohenSutherland(minX, minY, maxX, maxY);
}

void Line::draw(char **screen)
{
	float x1 = points[0]->x;
	float y1 = points[0]->y;
	float x2 = points[1]->x;
	float y2 = points[1]->y;

	float dx;
	float dy;

	dx = x2 - x1;
	dy = y2 - y1;

	float m;
	bool vert = false;

	if (y1 == y2)
	{
		m = 0;
	}
	else if (x1 == x2)
	{
		vert = true;
		m = 2;
	}
	else
	{
		m = (float)dy / (float)dx;
	}

	if (abs(m) < 1)
	{
		dx = 1;
		dy = m;
		int startx = 0;
		int endx = 0;
		float y;
		if (x1 > x2)
		{
			startx = x2;
			y = y2;
			endx = x1;
		}
		else
		{
			startx = x1;
			y = y1;
			endx = x2;
		}

		for (int x = startx; x <= endx; x++)
		{
			y += dy;
			screen[(int)y][x] = '-';
		}
	}
	else
	{
		dy = 1;
		if (vert)
			dx = 0;
		else
			dx = 1 / m;

		int starty = 0;
		int endy = 0;
		float x;
		if (y1 > y2)
		{
			starty = y2;
			x = x2;
			endy = y1;
		}
		else
		{
			starty = y1;
			x = x1;
			endy = y2;
		}
		for (int y = starty; y <= endy; y++)
		{
			x += dx;
			screen[y][(int)x] = '-';
		}
	}
}

int code(float x, float y, int minX, int minY, int maxX, int maxY)
{
	int code;

	code = inside;

	if (x < minX)
		code |= outLeft;
	else if (x > maxX)
		code |= outRight;
	if (y < minY)
		code |= outBottom;
	else if (y > maxY)
		code |= outTop;

	return code;
}

// Adapted from the wikipedia page (https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm)
bool Line::CohenSutherland(int minX, int minY, int maxX, int maxY)
{
	float x1 = points[0]->x;
	float y1 = points[0]->y;
	float x2 = points[1]->x;
	float y2 = points[1]->y;

	int code1 = code(x1, y1, minX, minY, maxX, maxY);
	int code2 = code(x2, y2, minX, minY, maxX, maxY);

	bool clipped = false;

	while (true) {
		if (!(code1 | code2))
		{
			// OR is 0 
			clipped = true;
			break;
		}
		else if (code1 & code2)
		{
			// AND is not 0
			break;
		}
		else
		{
			float x, y;

			int outcode = code1 ? code1 : code2;

			// find the intersection point with similar triangles
			if (outcode & outTop)
			{
				x = x1 + (x2 - x1) * (maxY - y1) / (y2 - y1);
				y = maxY;
			}
			else if (outcode & outBottom)
			{
				x = x1 + (x2 - x1) * (minY - y1) / (y2 - y1);
				y = minY;
			}
			else if (outcode & outRight)
			{
				y = y1 + (y2 - y1) * (maxX - x1) / (x2 - x1);
				x = maxX;
			}
			else if (outcode & outLeft)
			{
				y = y1 + (y2 - y1) * (minX - x1) / (x2 - x1);
				x = minX;
			}

			if (outcode == code1)
			{
				x1 = x;
				y1 = y;
				code1 = code(x1, y1, minX, minY, maxX, maxY);

			}
			else
			{
				x2 = x;
				y2 = y;
				code2 = code(x2, y2, minX, minY, maxX, maxY);
			}
		}
	}
	if (clipped)
	{
		points[0]->x = x1;
		points[0]->y = y1; 
		points[1]->x = x2; 
		points[1]->y = y2; 
	}

	return clipped;
}

