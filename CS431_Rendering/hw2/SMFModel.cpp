//
// Created by psarahdactyl on 4/10/16.
//

#include "SMFModel.h"
#include <map>
#include <utility>
#include <stdexcept>
#include <algorithm>

struct vec4Compare
{
	bool operator() (const Vector4D& lhs, const Vector4D& rhs)
	{
		if(lhs.x == rhs.x && lhs.y == rhs.y)
			return lhs.z < rhs.z;
     	else if (lhs.x == rhs.x)
       	  	return lhs.y < rhs.y;
		else
			return lhs.x < rhs.x;
	}
};

Vector3D* getNormal(Vector4D* a, Vector4D* b, Vector4D* c)
{
	Vector4D u = *b - *a;
	Vector4D v = *c - *a;

	Vector3D* normal = cross(u, v);
    normal->normalize();
	
	return normal;
}

void addToMap( Vector4D key, 
                Vector3D value , 
                std::map<Vector4D, std::vector<Vector3D>*, vec4Compare> &vertexNormals)
{
    std::vector<Vector3D>* valueArray;
	try
	{
		valueArray = vertexNormals.at( key );
		valueArray->push_back( value );
	}
	catch(const std::out_of_range& e) 
	{
		valueArray = new std::vector<Vector3D>();
		valueArray->push_back( value );
		vertexNormals.insert( std::pair<Vector4D, std::vector<Vector3D>*>(key, valueArray) );
	}
}

SMFModel::SMFModel(std::string name)
{
	this->tris = std::vector<Polygon>();
	this->parseSMFFile(name);
}

void SMFModel::parseSMFFile(std::string name)
{
    std::map<Vector4D, std::vector<Vector3D>*, vec4Compare> vertexNormals;

    std::string filename = "./";
	filename += name;

    std::vector<Vector4D*> vertices;
    std::vector<Vector3D*> normals;
    std::string line;

    std::ifstream infile( filename.c_str() );

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

            Vector3D *normal = getNormal(v1copy, v2copy, v3copy);
            normals.push_back(normal);

            poly->addVertex(v1copy);
	        addToMap(*v1copy, *normal, vertexNormals);
            poly->addVertex(v2copy);
	        addToMap(*v2copy, *normal, vertexNormals);
            poly->addVertex(v3copy);
	        addToMap(*v3copy, *normal, vertexNormals);

            tris.push_back(*poly);
        }
    }

	for(int j = 0; j < tris.size(); j++)
	{
        Polygon& tri = tris.at(j);
        for(int l = 0; l < tri.points.size(); l++)
        {
            //std::cout << l << std::endl;
            std::vector<Vector3D>* vNorms = vertexNormals.at( *tri.points.at(l) );
	        Vector3D *sum = new Vector3D(0,0,0);
	        for(unsigned int k = 0; k < vNorms->size(); k++)
	        {
	    	    *sum += vNorms->at(k);
	        }
			//*sum /= vNorms->size();
            sum->normalize();
	        tri.normals.push_back(sum);
        }
	}
}


std::pair<Vector4D*, Vector4D*> SMFModel::intersect(Ray r)
{
	std::vector<std::pair<float, std::pair<Vector4D*, Vector4D*> > > hits;
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

		Vector4D Na (tri.normals.at(0)->x, tri.normals.at(0)->y, tri.normals.at(0)->z, 0);
		Vector4D Nb (tri.normals.at(1)->x, tri.normals.at(1)->y, tri.normals.at(1)->z, 0);
		Vector4D Nc (tri.normals.at(2)->x, tri.normals.at(2)->y, tri.normals.at(2)->z, 0);

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

		float tNum = ((ax - bx)*(ay - cy)*(az - zo)) + 
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
			Vector4D* intersect = new Vector4D(xo + t * xd, yo + t * yd, zo + t * zd, 1);
            Vector4D* normal = new Vector4D();
            *normal = ((1-Beta-Gamma) * (Na)) + (Beta * (Nb)) + (Gamma * (Nc));
			normal->normalize();
			float dottyp = normal->dot(*normal, r.Rd);
			if (dottyp > 0)
			{
				*normal = -(*normal);
			}

			hits.push_back(std::pair<float, std::pair<Vector4D*, Vector4D*> >(t, std::pair<Vector4D*, Vector4D*>(intersect, normal)));
		}
	}
	if (hits.size() > 0)
	{
		int minIndex = 0;
		float minT = hits[0].first;
		for (int i = 1; i < hits.size(); i++)
		{
			float t = hits[i].first;
			if (t < minT)
			{
				minT = t;
				minIndex = i;
			}
		}
		return hits[minIndex].second;
	}

    return std::pair<Vector4D*, Vector4D*>(NULL, NULL);
}
