#ifndef POLYGON_H
#define POLYGON_H

#include "Vector.h"
#include "Shape.h"

class Polygon : public Shape
{
	public:
		Polygon();
		std::vector<Vector3D*> extremaPoints;
		void addVertex(int x, int y);
		virtual bool clip(int minX, int minY, int maxX, int maxY);
		virtual void draw(char ** screen);
		virtual void fill(char ** screen, int height, int width);
	private:
		bool SutherlandHodgman(int minX, int minY, int maxX, int maxY);
		void tester(int minX, int minY, int maxX, int maxY);
};


#endif

