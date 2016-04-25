#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <utility>
#include "Ray.h"

class Model
{
	public:
        virtual std::pair<Vector4D*, Vector4D*> intersect(Ray r) = 0;
        Vector4D* color = new Vector4D(1,1,1,1);
        float k_d = 0.7;
        float k_s = 0.3;
        float k_a = 0.3;
        int specular = 3;
};


#endif

