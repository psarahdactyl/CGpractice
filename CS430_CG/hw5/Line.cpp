#include "Line.h"
#include "Shape.h"
#include <iostream>
#include <cmath>

using namespace std;

Line::Line(float x1, float y1, float z1, float x2, float y2, float z2)
{
	Vector4D* point1 = new Vector4D(x1, y1, z1, 1.0);
	Vector4D* point2 = new Vector4D(x2, y2, z2, 1.0);
	points.push_back(point1);
	points.push_back(point2);
}

bool Line::clip(float prpZ, float front, float back, bool perspective) 
{
	return CohenSutherland(prpZ, front, back, perspective);
}

void Line::draw(float ** zBuff, char **screen, int height, int width, float zFront)
{

	float x1 = points[0]->x;
	float y1 = points[0]->y;
	float z1 = zVals[0];
	float x2 = points[1]->x;
	float y2 = points[1]->y;
	float z2 = zVals[1];

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
			float pz = (z1 - (z1 - z2)) * ( (y1 - y) / (y1 - y2) );
			if (y < 0 || x < 0 || y >= height || x >= width)
				continue;
			if (pz < zFront && pz > zBuff[(int)y][x])
			{
				zBuff[(int)y][x] = pz;
				screen[(int)y][x] = (char)baseColor;
			}
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
			float pz = (z1 - (z1 - z2)) * ( (y1 - y) / (y1 - y2) );
			if (y < 0 || x < 0 || y >= height || x >= width)
				continue;
			if (pz < zFront && pz > zBuff[y][(int)x])
			{
				zBuff[y][(int)x] = pz;
				screen[y][(int)x] = (char)baseColor;
			}
		}
	}
}

int codePerspective(float x, float y, float z, float PRPz, float front, float back)
{
	int code = inside;
	float zMin = (PRPz - front) / (back - PRPz);

	if (y > -z)
		code |= above;
	else if (y < z)
		code |= below;
	else if (x > -z)
		code |= rightOf;
	else if (x < z)
		code |= leftOf;
	else if (z < -1)
		code |= behind;
	else if (z > zMin)
		code |= inFront;

	return code;
}

int codeParallel(float x, float y, float z)
{
	int code = inside;

	if (y > 1)
		code |= above;
	else if (y < -1)
		code |= below;
	else if (x > 1)
		code |= rightOf;
	else if (x < -1)
		code |= leftOf;
	else if (z < -1)
		code |= behind;
	else if (z > 0)
		code |= inFront;

	return code;
}

int code(float x, float y, float z, float PRPz, float front, float back, bool perspective)
{

	int code;

	if (perspective)
	{
		code = codePerspective(x, y, z, PRPz, front, back);
	}
	else
	{
		code = codeParallel(x, y, z);
	}

	return code;
}

bool Line::CohenSutherland(float PRPz, float front, float back, bool perspective)
{
	float x1 = points[0]->x;
	float y1 = points[0]->y;
	float z1 = points[0]->z;
	float x2 = points[1]->x;
	float y2 = points[1]->y;
	float z2 = points[1]->z;

	int code1 = code(x1, y1, z1, PRPz, front, back, perspective);
	int code2 = code(x2, y2, z2, PRPz, front, back, perspective);

	bool clipped = false;

	return !(code1 | code2);

	/*
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
	*/
}

void Line::fill(float ** zBuff, char **screen, int height, int width)
{
}

