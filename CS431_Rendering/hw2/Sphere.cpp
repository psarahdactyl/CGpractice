//
// Created by psarahdactyl on 4/8/16.
//

#include "Sphere.h"
#include <cmath>
#include <algorithm>
#include <utility>

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

std::pair<Vector4D*, Vector4D*> Sphere::intersect(Ray r)
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
	float rs2 = pow(rs, 2);
    float C = pow((xo - xc), 2) + pow((yo - yc), 2) + pow((zo - zc), 2) - rs2;

    float D = pow(B, 2) - 4 * A * C;

    if(D < 0)
        return std::pair<Vector4D*, Vector4D*>(NULL, NULL);

    float t0 = ( -B - sqrt(D) ) / (2 * A);

	float ti = t0;
    if(t0 < 0.00001)
    {
        float t1 = ( -B + sqrt(D) ) / (2 * A);
		if(t1 < 0.00001)
			return std::pair<Vector4D*, Vector4D*>(NULL, NULL);
		ti = t1;
    }

	//if(ti > -0.0001 && ti < 0.0001)
        //return std::pair<Vector4D*, Vector4D*>(NULL, NULL);

    Vector4D* intersect = new Vector4D(xo + xd * ti, yo + yd * ti, zo + zd * ti, 0);
	/*
    float div = 1.0 / rs;
    intersect->x = intersect->x * div;
    intersect->y = intersect->y * div;
    intersect->z = intersect->z * div;
	*/

    Vector4D* center = new Vector4D(this->x, this->y, this->z, 0);
    Vector4D* normal = new Vector4D();
    *normal = *intersect - *center; 
    float div = 1.0 / rs;
	*normal *= div;
	normal->normalize();

    return std::pair<Vector4D*, Vector4D*>(intersect, normal);

}


