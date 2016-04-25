//
// Created by psarahdactyl on 4/10/16.
//

#ifndef HW1_SMFMODEL_H
#define HW1_SMFMODEL_H


#include "Model.h"
#include "Polygon.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iterator>
#include <iomanip>

class SMFModel : public Model
{
    public:
		SMFModel(std::string name);
		std::vector<Polygon> tris;
        void parseSMFFile(std::string name);
		Vector4D* intersect(Ray r);

};


#endif //HW1_SMFMODEL_H
