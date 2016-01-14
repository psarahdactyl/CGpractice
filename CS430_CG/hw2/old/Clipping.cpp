#include "Clipping.h"
#include <iostream>

// Adapted from the wikipedia page (https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm)

using namespace std;

int inside = 0; 	// 0000
int outLeft = 1;	// 0001
int outRight = 2;	// 0010
int outBottom = 4;	// 0100
int outTop = 8;		// 1000

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

Line* Clipping::CohenSutherland( Line l, int minX, int minY, int maxX, int maxY )
{
	int x1 = l.point1.x;
	int y1 = l.point1.y;
	int x2 = l.point2.x;
	int y2 = l.point2.y;

	int code1 = code(x1, y1, minX, minY, maxX, maxY);
	int code2 = code(x2, y2, minX, minY, maxX, maxY);

	bool clipped = false;

	while(true) {
		if(!(code1 | code2)) 
		{ 
			// OR is 0 
			clipped = true;
			break;
		} 
		else if(code1 & code2) 
		{ 
			// AND is not 0
			break;
		} 
		else 
		{
			float x, y;

			int outcode = code1 ? code1 : code2;

			// find the intersection point with similar triangles
			if(outcode & outTop) 
			{ 
				x = x1 + (x2 - x1) * (maxY - y1) / (y2 - y1);
				y = maxY;
			} 
			else if(outcode & outBottom) 
			{ 
				x = x1 + (x2 - x1) * (minY - y1) / (y2 - y1);
				y = minY;
			} 
			else if(outcode & outRight) 
			{  
				y = y1 + (y2 - y1) * (maxX - x1) / (x2 - x1);
				x = maxX;
			} 
			else if(outcode & outLeft) 
			{ 
				y = y1 + (y2 - y1) * (minX - x1) / (x2 - x1);
				x = minX;
			}

			if(outcode == code1) 
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
	if(clipped) 
	{
		return new Line(x1, y1, x2, y2);
	}
	else
	{
		return NULL;
	}
}

