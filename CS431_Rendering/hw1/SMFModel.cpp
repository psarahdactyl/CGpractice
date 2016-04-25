//
// Created by psarahdactyl on 4/10/16.
//

#include "SMFModel.h"

SMFModel::SMFModel(std::string name)
{
	this->tris = std::vector<Polygon>();
	this->parseSMFFile(name);
}

void SMFModel::parseSMFFile(std::string name)
{
    std::string filename = "./";
	filename += name;

    std::vector<Vector4D*> vertices;
    std::string line;

    std::ifstream infile( filename.c_str() );
    std::cout << infile.is_open() << std::endl;

    int i = 0;
    while (std::getline( infile, line )) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             std::back_inserter(tokens));

        if (tokens.size() == 0)
            continue;

        else if (tokens.at(0) == "v") {
            float x, y, z;
            std::stringstream datax(tokens.at(1));
            datax >> x;
            std::stringstream datay(tokens.at(2));
            datay >> y;
            std::stringstream dataz(tokens.at(3));
            dataz >> z;
            vertices.push_back(new Vector4D(x, y, z, 1.0));
            i++;
        }
        else if (tokens.at(0) == "f") {
            int v1, v2, v3;
            std::stringstream datav1(tokens.at(1));
            datav1 >> v1;
            std::stringstream datav2(tokens.at(2));
            datav2 >> v2;
            std::stringstream datav3(tokens.at(3));
            datav3 >> v3;

            Polygon *poly = new Polygon();
            Vector4D *vector1 = vertices[v1 - 1];
            Vector4D *v1copy = new Vector4D(vector1->x, vector1->y, vector1->z, 1);
            Vector4D *vector2 = vertices[v2 - 1];
            Vector4D *v2copy = new Vector4D(vector2->x, vector2->y, vector2->z, 1);
            Vector4D *vector3 = vertices[v3 - 1];
            Vector4D *v3copy = new Vector4D(vector3->x, vector3->y, vector3->z, 1);

            poly->addVertex(v1copy);
            poly->addVertex(v2copy);
            poly->addVertex(v3copy);
            tris.push_back(*poly);
        }
    }
}

Vector4D* SMFModel::intersect(Ray r)
{
	for (int i = 0; i < tris.size(); i++)
	{
		Polygon tri = tris.at(i);
		float ax = tri.points.at(0)->x;
		float ay = tri.points.at(0)->y;
		float az = tri.points.at(0)->z;
		float bx = tri.points.at(1)->x;
		float by = tri.points.at(1)->y;
		float bz = tri.points.at(1)->z;
		float cx = tri.points.at(2)->x;
		float cy = tri.points.at(2)->y;
		float cz = tri.points.at(2)->z;

		float xo = r.Ro.x;
		float yo = r.Ro.y;
    	float zo = r.Ro.z;
    	float xd = r.Rd.x;
    	float yd = r.Rd.y;
    	float zd = r.Rd.z;

		float denom = ((ax - bx)*(ay - cy)*(zd)) + 
						((ax - cx)*(yd)*(az - bz)) + 
						((xd)*(ay - by)*(az - cz)) - 
						((xd)*(ay - cy)*(az - bz)) - 
						((ax - cx)*(ay - by)*(zd)) - 
						((ax - bx)*(yd)*(az - cz));

		float betaNum = ((ax - xo)*(ay - cy)*(zd)) +
						((ax - cx)*(yd)*(az - zo)) + 
						((xd)*(ay - yo)*(az - cz)) - 
						((xd)*(ay - cy)*(az - zo)) - 
						((ax - cx)*(ay - yo)*(zd)) - 
						((ax - xo)*(yd)*(az - cz));
		float Beta = betaNum / denom;

		float gammaNum = ((ax - bx)*(ay - yo)*(zd)) + 
						((ax - xo)*(yd)*(az - bz)) + 
						((xd)*(ay - by)*(az - zo)) - 
						((xd)*(ay - yo)*(az - bz)) - 
						((ax - xo)*(ay - by)*(zd)) - 
						((ax - bx)*(yd)*(az - zo));
		float Gamma = gammaNum / denom;

		float tNum = ((ax - bx)*(ay - cy)*(ax - zo)) + 
						((ax - cx)*(ay - yo)*(az - bz)) + 
						((ax - xo)*(ay - by)*(az - cz)) - 
						((ax - xo)*(ay - cy)*(az - bz)) - 
						((ax - cx)*(ay - by)*(az - zo)) - 
						((ax - bx)*(ay - yo)*(az - cz));
		float t = tNum / denom;

		if (Beta >= 0 && Gamma >= 0
		  && Beta + Gamma <= 1
		  && t >= 0)
		{
			Vector4D* ret = new Vector4D(xo + t * xd, yo + t * yd, zo + t * zd, 1);
			return ret;
		}
	}
	return 0;
}
