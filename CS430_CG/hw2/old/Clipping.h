#ifndef CLIPPING_H
#define CLIPPING_H

#include "Line.h"

enum clipper {CohenSutherland, SutherlandHodgman};

class Clipping
{
	public:
		static Line* CohenSutherland( Line l, int minX, int minY, int maxX, int maxY );
};


#endif
