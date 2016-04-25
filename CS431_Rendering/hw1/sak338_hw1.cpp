// Sarah Kushner
// CS 431
// HW 1

#define _USE_MATH_DEFINES

#include "lodepng.h"

#include "Camera.h"
#include "Sphere.h"
#include "SMFModel.h"

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
    vector<Model*> scene;

	SMFModel* m1 = new SMFModel("bound-cow.smf");
	scene.push_back(m1);
    Sphere* s1 = new Sphere(0,0,1,0.4);
    scene.push_back(s1);
    Sphere* s2 = new Sphere(0,0,-1,0.4);
    scene.push_back(s2);

    c->traceRays(scene);

    const char* filename = argc > 1 ? argv[1] : "test.png";
    encodeOneStep(filename, c->pixels, 512, 512);

	return 0;
}

