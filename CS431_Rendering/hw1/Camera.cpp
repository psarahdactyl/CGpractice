//
// Created by psarahdactyl on 4/8/16.
//

#include "Camera.h"
#include <iostream>

#define PI 3.14159265

Camera::Camera()
{
    this->eyePoint = new Vector4D(3,0,0,1);
    this->Zv = new Vector4D(-1,0,0,1);
    this->Vup = new Vector4D(0,1,0,1);
    this->cameraAngle = 56;
    this->hres = 512;
    this->vres = 512;
    this->Xv = Vup->cross(*Zv);
    this->Yv = Zv->cross(*Xv);
    this->Xv->normalize();
    this->Yv->normalize();
    this->Zv->normalize();
    this->d = 10;
    float h = d * tan( (cameraAngle/2) * PI / 180.0);
    this->Sj = 2 * h;
    this->Sk = Sj;
    Vector4D* v = new Vector4D();
    *v = (*eyePoint) +
         d * (*Zv) -
         (Sj / 2) * (*Xv) +
         (Sk / 2) * (*Yv);
    this->P00 = v;
}

Camera::Camera(Vector4D* eyePoint, Vector4D* Zv, Vector4D* Vup, float cameraAngle, int hres, int vres, int d)
{
    this->eyePoint = eyePoint;
    this->Zv = Zv;
    this->Vup = Vup;
    this->cameraAngle = cameraAngle;
    this->hres = hres;
    this->vres = vres;
    this->Xv = Vup->cross(*Zv);
    this->Yv = Zv->cross(*Xv);
    this->Xv->normalize();
    this->Yv->normalize();
    this->Zv->normalize();
    this->d = d;
    float h = this->d * tan( (cameraAngle/2) * PI / 180.0);
    this->Sj = 2 * h;
    this->Sk = Sj * (vres / hres);
    Vector4D* v = new Vector4D();
    *v = (*eyePoint) +
        d * (*Zv) -
        (Sj / 2) * (*Xv) +
        (Sk / 2) * (*Yv);
    this->P00 = v;
}

void Camera::traceRays(std::vector<Model*> scene)
{
    std::vector<unsigned char> image;
    image.resize(hres * vres * 4);
    for(float j = 0; j < hres; j++) {
        for(float k = 0; k < vres; k++) {
            Vector4D djk = ((*P00) +
                            Sj * (j / (hres - 1)) * (*Xv) -
                            Sk * (k / (vres - 1)) * (*Yv) -
                            (*eyePoint));
            djk.normalize();
			djk.z = -djk.z;
            Ray* r = new Ray(*eyePoint, djk, 0);
            rays.push_back(r);
            for (int i = 0; i < scene.size(); ++i) {
                if(scene.at(i)->intersect(*r) == 0)
                {
                    image[4 * hres * k + 4 * j + 0] = 0;     // R
                    image[4 * hres * k + 4 * j + 1] = 0;     // G
                    image[4 * hres * k + 4 * j + 2] = 0;     // B
                    image[4 * hres * k + 4 * j + 3] = 255;     // A
                }
                else
                {
                    image[4 * hres * k + 4 * j + 0] = 255;     // R
                    image[4 * hres * k + 4 * j + 1] = 255;     // G
                    image[4 * hres * k + 4 * j + 2] = 255;     // B
                    image[4 * hres * k + 4 * j + 3] = 255;   // A
					break;

                }
            }
        }
    }
    this->pixels = image;
}



