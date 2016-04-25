#ifndef SPHERE_H
#define SPHERE_H

#include "Vector4D.h"
#include "Model.h"

class Sphere : public Model
{
	public:
		float x;
        float y;
        float z;
        float r;
        Vector4D center;
        Sphere();
		Sphere(float x, float y, float z, float r);
		std::pair<Vector4D*, Vector4D*> intersect(Ray r);
};


#endif

