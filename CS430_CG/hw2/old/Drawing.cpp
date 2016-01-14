#include "Drawing.h"
#include <iostream>
#include <cmath>

using namespace std;

char ** Drawing::DDA( Line l, char ** screen )
{
	int x1 = l.point1.x;
	int y1 = l.point1.y;
	int x2 = l.point2.x;
	int y2 = l.point2.y;

	float dx;
	float dy;

	dx = x2 - x1;
   	dy = y2 - y1;
	 
	float m;
	bool vert = false;

    if(y1 == y2)
    {
        m = 0;
    }
    else if(x1 == x2)
    {
		vert = true;
		m = 2;
    }
	else
	{
        m = (float)dy / (float)dx;
	}

	if(abs(m) < 1)
    {
		dx = 1;
		dy = m;
		int startx = 0;
    	int endx   = 0;
		float y; 
    	if(x1 > x2)
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

    	for(int x = startx; x <= endx; x++)
    	{
    		y += dy;
			//cout << x << endl << y << endl;
    		screen[(int)y][x] = '-';
    	}
	}
	else
    {
    	dy = 1;
		if(vert)
			dx = 0;
		else
    		dx = 1 / m;

		int starty = 0;
    	int endy   = 0;
		float x;
    	if(y1 > y2)
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
		for(int y = starty; y <= endy; y++)
		{
      		x += dx;
			//cout << x << endl << y << endl;
        	screen[y][(int)x] = '-';
	  	} 
    }

    return screen;
}
