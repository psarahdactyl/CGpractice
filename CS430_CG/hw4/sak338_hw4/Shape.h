#ifndef SHAPE_H
#define SHAPE_H

#include "Vector4D.h"
#include <vector>

class Shape 
{
	public:
		std::vector<Vector4D*> points;
		virtual bool clip(float prpZ, float front, float back, bool perspective) = 0;
		//void transform(float scaleFactorX, float scaleFactorY, float radians, int translateX, int translateY);
		//void worldToScreen(int lowerBoundX, int lowerBoundY, int upperBoundY);
		void worldToViewport(int xmin, int xmax, int ymin, int ymax, 
							int umin, int umax, int vmin, int vmax);
		virtual void draw(char ** screen, int height, int width) = 0;
		//virtual void fill(char ** screen, int height, int width) = 0;
};


#endif
