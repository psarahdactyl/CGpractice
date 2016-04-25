//
// Created by psarahdactyl on 4/8/16.
//

#include <vector>
#include "Ray.h"

Ray::Ray()
{
    this->Ro = Vector4D(0,0,0,1);
    this->Rd = Vector4D(0,0,0,1);
    this->t = 0;

}

Ray::Ray(Vector4D Ro, Vector4D Rd, float t)
{
    this->Ro = Ro;
    this->Rd = Rd;
    this->t = t;

}





