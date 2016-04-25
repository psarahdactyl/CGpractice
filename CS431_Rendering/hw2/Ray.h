#ifndef RAY_H
#define RAY_H

#include "Vector4D.h"
#include "Vector3D.h"

class Ray
{
	public:
		Vector4D Ro;
        Vector4D Rd;
        float t;
		Ray();
		Ray(Vector4D Ro, Vector4D Rd, float t);
};


#endif

