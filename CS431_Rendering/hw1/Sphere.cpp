//
// Created by psarahdactyl on 4/8/16.
//

#include "Sphere.h"
#include <cmath>
#include <algorithm>

Sphere::Sphere()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->r = 0.4;
}

Sphere::Sphere(float x, float y, float z, float r)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->center = Vector4D(x, y, z, 1);
}

Vector4D* Sphere::intersect(Ray r)
{
    float xc = x;
    float yc = y;
    float zc = z;
    float xo = r.Ro.x;
    float yo = r.Ro.y;
    float zo = r.Ro.z;
    float xd = r.Rd.x;
    float yd = r.Rd.y;
    float zd = r.Rd.z;
    float rs = this->r;

    float A = pow(xd, 2) + pow(yd, 2) + pow(zd, 2);
    float B = 2 * ( xd * (xo - xc) + yd * (yo - yc) + zd * (zo - zc) );
    float C = pow((xo - xc), 2) + pow((yo - yc), 2) + pow((zo - zc), 2) - pow(rs, 2);

    float D = pow(B, 2) - 4 * A * C;

    if(D < 0)
        return 0;

    float t0 = ( -B - sqrt(D) ) / (2 * A);
    float t1;

    if(t0 < 0 - 0.0001)
    {
        t1 = ( -B + sqrt(D) ) / (2 * A);
    }

    float ti = std::min(t0, t1);

    if(ti > 0 - 0.0001 && ti < 0 + 0.0001)
        return 0;

    Vector4D* ret = new Vector4D(xo + xd * ti, yo + yd * ti, zo + zd * ti, 1);
    float div = 1 / rs;
    ret->x = ret->x * div;
    ret->x = ret->y * div;
    ret->x = ret->z * div;

    return ret;
}


