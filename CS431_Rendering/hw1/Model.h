#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Ray.h"

class Model
{
	public:
        virtual Vector4D* intersect(Ray r) = 0;

};


#endif

