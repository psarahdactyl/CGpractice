//
// Created by psarahdactyl on 4/22/16.
//

#ifndef HW2_LIGHT_H
#define HW2_LIGHT_H

#include "Vector4D.h"

class Light {

    public:
        Light();
        Light(Vector4D* position, Vector4D* diffuse, Vector4D* ambient, Vector4D* specular);

	    Vector4D* position;
	    Vector4D* I_d;
	    Vector4D* I_a;
	    Vector4D* I_s;
};


#endif //HW2_LIGHT_H
