#ifndef SHAPE_H
#define SHAPE_H

#include "Vector.h"
#include <vector>

class Shape 
{
	public:
		std::vector<Vector3D*> points;
		virtual bool clip(int minX, int minY, int maxX, int maxY) = 0;
		void transform(float scaleFactor, float radians, int translateX, int translateY);
		void worldToScreen(int lowerBoundX, int lowerBoundY, int upperBoundY);
		virtual void draw(char ** screen) = 0;
};


#endif
