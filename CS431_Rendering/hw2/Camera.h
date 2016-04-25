#ifndef CAMERA_H
#define CAMERA_H

#include "Vector4D.h"
#include "Model.h"
#include "Light.h"
#include <cmath>
#include <utility>

class Camera
{
	public:
		Vector4D* Xv;
		Vector4D* Yv;
        Vector4D* Zv;
        Vector4D* eyePoint;
        Vector4D* Vup;
        Vector4D* P00;
        float cameraAngle;
		float hres;
		float vres;
		float Sj;
		float Sk;
		float d;
		std::vector<Ray*> rays;
		std::vector<unsigned char> pixels;
        Camera();
		Camera(Vector4D* eyePoint, Vector4D* Zv, Vector4D* Vup, float cameraAngle, int hres, int vres, int d);
		void supersample();
		void traceRays(std::vector<Model*> scene, std::vector<Light*> lights);
};


#endif

