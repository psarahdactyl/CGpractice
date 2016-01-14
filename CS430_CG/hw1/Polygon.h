#ifndef POLYGON_H
#define POLYGON_H

#include "Vector.h"
#include "Shape.h"

class Polygon : public Shape
{
public:
	Polygon();
	void addVertex(int x, int y);
	virtual bool clip(int minX, int minY, int maxX, int maxY);
	virtual void draw(char ** screen);
private:
	bool SutherlandHodgman(int minX, int minY, int maxX, int maxY);
	void tester(int minX, int minY, int maxX, int maxY);
};


#endif

