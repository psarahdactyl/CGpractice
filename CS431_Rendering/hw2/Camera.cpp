//
// Created by psarahdactyl on 4/8/16.
//

#include "Camera.h"
#include <iostream>
#include <algorithm>
#include <cmath>

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

Vector4D calculateColor(Light* l, Model* m, Vector4D* intersect, Vector4D* normal, Vector4D* eyePoint)
{
	
    Vector4D* obj_color = m->color;

    Vector4D L = *(l->position) - *intersect;

    Vector4D V  = *eyePoint - *intersect;

    L.normalize();
    V.normalize();

    float nL = std::max(0.0f, normal->dot(*normal, L));

    Vector4D R = (2 * nL * (*normal)) - L;
    
    float rV = std::pow(std::max(0.0f, R.dot(R, V)), m->specular);

    Vector4D diffuse_comp = *(l->I_d) * (*obj_color) * m->k_d * nL;
    Vector4D specular_comp = *(l->I_s) * (*obj_color) * m->k_s * rV;
    Vector4D ambient_comp = *(l->I_a) * (*obj_color) * m->k_a;
    if (normal->dot(*normal, L) < 0)
    	specular_comp = Vector4D(0, 0, 0, 1);
    
    float small = 0.000001;
    if (nL > small || rV > small)
    	int i =0;
    
    Vector4D color = diffuse_comp + specular_comp + ambient_comp;
    
    return color;

}

void Camera::supersample()
{
	std::vector<unsigned char> newimage;
	newimage.resize((hres/2) * (vres/2) * 4);
	for (int i = 0; i < hres / 2; i++)
	{
		for (int j = 0; j < vres / 2; j++)
		{
			int offsetx = 2 * i;
			int offsety = 2 * j;
			newimage[4 * (hres/2) * j + 4 * i + 0] =
				(pixels[4 * hres * offsety + 4 * (offsetx + 1) + 0] +
				pixels[4 * hres * (offsety + 1) + 4 * (offsetx) + 0] +
				pixels[4 * hres * (offsety + 1) + 4 * (offsetx + 1) + 0]) / 4;     // R
			newimage[4 * (hres / 2) * j + 4 * i + 1] =
				(pixels[4 * hres * offsety + 4 * (offsetx + 1) + 1] +
				pixels[4 * hres * (offsety + 1) + 4 * (offsetx) + 1] +
				pixels[4 * hres * (offsety + 1) + 4 * (offsetx + 1) + 1]) / 4;     // G
			newimage[4 * (hres / 2) * j + 4 * i + 2] =
				(pixels[4 * hres * offsety + 4 * (offsetx + 1) + 2] +
				pixels[4 * hres * (offsety + 1) + 4 * (offsetx) + 2] +
				pixels[4 * hres * (offsety + 1) + 4 * (offsetx + 1) + 2]) / 4;     // B
			newimage[4 * (hres / 2) * j + 4 * i + 3] = 255;                        // A

		}
	}
	pixels = newimage;
}

void Camera::traceRays(std::vector<Model*> scene, std::vector<Light*> lights)
{
    std::vector<unsigned char> image;
    image.resize(hres * vres * 4);
    for(float j = 0; j < hres; j++) {
		std::cout << j << std::endl;
        for(float k = 0; k < vres; k++) {
            Vector4D djk = ((*P00) +
                            Sj * (j / (hres - 1)) * (*Xv) -
                            Sk * (k / (vres - 1)) * (*Yv) -
                            (*eyePoint));
			if (j == 200 && k == 260)
				int i = 0;
            djk.normalize();
			djk.z = -djk.z;
            Ray* r = new Ray(*eyePoint, djk, 0);
            rays.push_back(r);

            for (int i = 0; i < scene.size(); ++i) {
                std::pair<Vector4D*, Vector4D*> p = scene.at(i)->intersect(*r);
                if(p.first == NULL && p.second == NULL)
                {
                    image[4 * hres * k + 4 * j + 0] = 0;     // R
                    image[4 * hres * k + 4 * j + 1] = 0;     // G
                    image[4 * hres * k + 4 * j + 2] = 0;     // B
                    image[4 * hres * k + 4 * j + 3] = 255;     // A
                }
                else
                {
					Vector4D color(0, 0, 0, 1);
                    for(int l = 0; l < lights.size(); l++)
                    {
                        color += calculateColor(lights.at(l), scene.at(i), p.first, p.second, eyePoint);
                        int red = color.x * 255;
                        int green = color.y * 255;
                        int blue = color.z * 255;

						if (red < 0 || red > 255)
							red = 255;
						if (green < 0 || green > 255)
							green = 255;
						if (blue < 0 || blue > 255)
							blue = 255;
                        image[4 * hres * k + 4 * j + 0] = red;     // R
                        image[4 * hres * k + 4 * j + 1] = green;     // G
                        image[4 * hres * k + 4 * j + 2] = blue;     // B
                        image[4 * hres * k + 4 * j + 3] = 255;   // A
                    }
					break;

                }
            }
        }
    }
    this->pixels = image;
}



