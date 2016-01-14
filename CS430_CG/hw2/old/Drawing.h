#ifndef DRAWING_H
#define DRAWING_H

#include "Line.h"

class Drawing
{
	public:
		static char ** DDA( Line l, char ** screen );
};


#endif