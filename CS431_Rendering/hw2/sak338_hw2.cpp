// Sarah Kushner
// CS 431
// HW 1

#define _USE_MATH_DEFINES

#include "lodepng.h"

#include "Camera.h"
#include "Sphere.h"
#include "SMFModel.h"
#include "Light.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <cstring>
#include <iterator>
#include <iomanip>

using namespace std;

//Example 1
//Encode from raw pixels to disk with a single function call
//The image argument has width * height RGBA pixels or width * height * 4 bytes
void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
    //Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

    //if there's an error, display it
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

int main( int argc, char **argv )
{

    Camera* c = new Camera();
    c->hres = 1024;
    c->vres = 1024;
    vector<Model*> scene;
    vector<Light*> lights;

	SMFModel* m1 = new SMFModel("bound-bunny_200.smf");
    m1->k_d = 0.7;
    m1->k_s = 0.3;
    m1->specular = 1;
	m1->color = new Vector4D(0.5, 0.1, 0.6, 1);
	scene.push_back(m1);

	SMFModel* m2 = new SMFModel("penguin.smf");
    m2->color = new Vector4D(0.1,0.6,1,1);
    m2->k_d = 0.7;
    m2->k_s = 0.3;
    m2->specular = 40;
	scene.push_back(m2);

    Sphere* s1 = new Sphere(0,0,1,0.4);
	s1->specular = 1;
	s1->color = new Vector4D(1, 0.3, 0.2, 1);
    scene.push_back(s1);
	
    Sphere* s2 = new Sphere(0,0,-1,0.4);
	s2->specular = 30;
	s2->color = new Vector4D(0.2, 1, 0.4, 1);
    scene.push_back(s2);

    Light* l1 = new Light();
    lights.push_back(l1);

    c->traceRays(scene, lights);

    //c->supersample();

    const char* filename = argc > 1 ? argv[1] : "test.png";
    encodeOneStep(filename, c->pixels, 1024,1024);

	return 0;
}

