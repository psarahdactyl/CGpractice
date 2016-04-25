//
// Created by psarahdactyl on 4/22/16.
//

#include "Light.h"

Light::Light()
{
    this->position = new Vector4D( 1,1,1,1 );
    this->I_d = new Vector4D(0.5, 0.5, 0.5, 1.0);
	this->I_a = new Vector4D(0.5, 0.5, 0.5, 1.0);
	this->I_s = new Vector4D( 0.2, 0.2, 0.2, 1.0 );
}

Light::Light(Vector4D* position, Vector4D* diffuse, Vector4D* ambient, Vector4D* specular)
{
    this->position = position; 
    this->I_d = diffuse;
	this->I_a = ambient;
	this->I_s = specular;
}
