// Sarah Kushner
// CS 430
// HW 4

#define _USE_MATH_DEFINES

#include "Shape.h"
#include "Line.h"
#include "Polygon.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4D.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iterator>

using namespace std;

vector<Polygon*> shapes;

// taken from stack overflow question 
// http://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool isNumber(string line)
{
	char* p;
	strtol(line.c_str(), &p, 10);
	return *p == 0;
}

bool isFloat(string line)
{
	char* p;
	strtod(line.c_str(), &p);
	return *p == 0;
}

void parseSMFFile(string name)
{
	ifstream infile;

	string filename = "./";
	string line;

	filename += name;
	infile.open(filename.c_str());
	
	vector<Vector4D*> vertices;
	//vector<Vector4D*> points;
	//normals = new vec3[pointsSize];
	//Vector4D* colors = new Vector4D[pointsSize];

	infile.close();
	infile.open( filename );
	
	//cout << pointsSize << endl;

	int i = 0;
	while (getline( infile, line ))
	{
		istringstream iss( line );
		vector<string> tokens;
		copy(istream_iterator<string>(iss),
			 istream_iterator<string>(),
			 back_inserter(tokens));

		if(tokens.size() == 0)
			continue;

		else if(tokens.at(0) == "v")
		{
			float x, y, z;
			stringstream datax(tokens.at(1));
			datax >> x;
			stringstream datay(tokens.at(2));
			datay >> y;
			stringstream dataz(tokens.at(3));
			dataz >> z;
			vertices.push_back(new Vector4D(x, y, z, 1.0));
			i++;
		}
		else if(tokens.at(0) == "f")
		{
			int v1, v2, v3;	
			stringstream datav1(tokens.at(1));
			datav1 >> v1;
			stringstream datav2(tokens.at(2));
			datav2 >> v2;
			stringstream datav3(tokens.at(3));
			datav3 >> v3;

			//tri(v1-1, v2-1, v3-1);
			Polygon* poly = new Polygon();
			Vector4D* vector1 = vertices[v1 - 1];
			Vector4D* v1copy = new Vector4D(vector1->x, vector1->y, vector1->z, 1);
			Vector4D* vector2 = vertices[v2 - 1];
			Vector4D* v2copy = new Vector4D(vector2->x, vector2->y, vector2->z, 1);
			Vector4D* vector3 = vertices[v3 - 1];
			Vector4D* v3copy = new Vector4D(vector3->x, vector3->y, vector3->z, 1);

			poly->addVertex(v1copy);
			poly->addVertex(v2copy);
			poly->addVertex(v3copy);
			shapes.push_back(poly);
		}
	}
}

Matrix4D makeViewMatrix(Vector3D* VPN, Vector3D* VUP, Vector3D* VRR, Vector3D* VRP)
{
	Vector4D* VRR4 = new Vector4D(VRR->x, VRR->y, VRR->z, 0);
	Vector4D* VUP4 = new Vector4D(VUP->x, VUP->y, VUP->z, 0);
	Vector4D* VPN4 = new Vector4D(VPN->x, VPN->y, VPN->z, 0);
	Vector4D* H = new Vector4D(0, 0, 0, 1);
	Matrix4D* rotation = new Matrix4D(*VRR4, 
								      *VUP4, 
									  *VPN4, 
								      *H);
	*VRP = *VRP * -1;
	Matrix4D* translate = new Matrix4D(1,0,0,VRP->x,
									   0,1,0,VRP->y,
									   0,0,1,VRP->z,
									   0,0,0,1);
	Matrix4D view = *rotation * *translate;
	return view;
}

Matrix4D makeProjectionMatrix(Vector3D* PRP, float uMax,
							float uMin, float vMax, float vMin, 
							float front, float back, bool perspective)
{
	if (perspective)
	{
		float a = (2 * PRP->z) / ((uMax - uMin)*(PRP->z - back));
		float b = (2 * PRP->z) / ((vMax - vMin)*(PRP->z - back));
		float c = ((uMax + uMin) - (2 * PRP->x)) / ((uMax - uMin)*(PRP->z - back));
		float d = ((vMax + vMin) - (2 * PRP->y)) / ((vMax - vMin)*(PRP->z - back));
		float e = 1 / (PRP->z - back);
		float f = (-1 * (uMax + uMin) * PRP->z) / ((uMax - uMin)*(PRP->z - back));
		float g = (-1 * (vMax + vMin) * PRP->z) / ((vMax - vMin)*(PRP->z - back));
		float h = (-1 * PRP->z) / (PRP->z - back);
		Matrix4D* projection = new Matrix4D(a, 0, c, f,
											0, b, d, g,
											0, 0, e, h,
											0, 0, 0, 1);
		return *projection;
	}
	else
	{
		float a = 2 / (uMax - uMin);
		float b = 2 / (vMax - vMin);
		float c = ((uMax + uMin) - (2 * PRP->x)) / ((uMax - uMin)*(PRP->z));
		float d = ((vMax + vMin) - (2 * PRP->y)) / ((vMax - vMin)*(PRP->z));
		float e = (-1 * (uMax + uMin)) / 2;
		float f = 1 / (front - back);
		float g = (-1 * (vMax + vMin)) / 2;
		float h = (-1 * front) / (front - back);
		Matrix4D* projection = new Matrix4D(a, 0, c, e,
											0, b, d, g,
											0, 0, f, h,
											0, 0, 0, 1);
		return *projection;
	}

}

void convert(Matrix4D view, Matrix4D projection)
{
	for (Polygon* p : shapes)
	{
		p->convert(view, projection);
	}
}

void clip(float PRPz, float front, float back, bool perspective)
{
	for (int i = 0; i < shapes.size(); i++)
	{
		Shape* s = shapes[i];
		bool success = s->clip(PRPz, front, back, perspective);
		if (!success)
		{
			shapes.erase(shapes.begin() + i);
			i--;
		}
	}
}

void projectOnto2D(bool perspective)
{
	for (Polygon* p : shapes)
	{
		p->project(perspective);
	}
}

void worldToViewport(int lowerBoundX, int upperBoundX, int lowerBoundY, int upperBoundY,
					int viewLowerBoundX, int viewUpperBoundX, int viewLowerBoundY, int viewUpperBoundY)
{
	for (Shape* s : shapes)
		s->worldToViewport(lowerBoundX, upperBoundX, lowerBoundY, upperBoundY,
							viewLowerBoundX, viewUpperBoundX, viewLowerBoundY, viewUpperBoundY);
}

/*
void worldToScreen(int lowerBoundX, int lowerBoundY, int upperBoundY)
{
	for (Shape* s : shapes)
		s->worldToScreen(lowerBoundX, lowerBoundY, upperBoundY);
}
*/

void outputXPMFile(int lowerBoundX, int lowerBoundY, int upperBoundX, int upperBoundY)
{
	int width = 501;
	int height = 501;

	char **screen; // [height][width];

	screen = new char*[height];
	for(int i = 0; i < height; i ++)
	{
		screen[i] = new char[width];
		for(int j = 0; j < width; j++)
		{
			screen[i][j] = '#';
		}
	}

	for (Shape* s : shapes)
	{
		s->draw(screen, height, width);
		//s->fill(screen, height, width);
	}

	cout << "/* XPM */" << endl;
	cout << "static char *sco1000[] = {" << endl;
	cout << "/* width height num_colors chars_per_pixel */" << endl;
	cout << "\"" << width << " " << height << " 2 1\"," << endl;
	cout << "/* colors */" << endl;
	cout << "\"# c #ffffff\"," << endl;
	cout << "\"- c #000000\"," << endl;
	cout << "/* pixels */" << endl;
	cout.flush();

	for(int i = 0; i < height; i ++)
	{
		cout << "\"";
		for(int j = 0; j < width; j++)
		{
			cout << screen[i][j];
		}
		if(i < height - 1)
			cout << "\"," << endl;
	}

	cout << "};" << endl;

}

int main( int argc, char **argv )
{
	//view point bounds 
	int viewLowerBoundX = 0;
	int viewLowerBoundY = 0;
	int viewUpperBoundX = 500;
	int viewUpperBoundY = 500;
	
	//projection reference point
	float prpX = 0;
	float prpY = 0;
	float prpZ = 1;

	//view refernce point 
	float vrpX = 0;
	float vrpY = 0;
	float vrpZ = 0;

	//view plane normal
	float vpnX = 0;
	float vpnY = 0;
	float vpnZ = -1;

	//view up vector 
	float vupX = 0;
	float vupY = 1;
	float vupZ = 0;

	//bounds for vrc window
	float uMin = -0.7;
	float vMin = -0.7;
	float uMax = 0.7;
	float vMax = 0.7;

	//persepective duh
	bool perspective = true;

	//front and back clipping planes
	float back = -0.6;
	float front = 0.6;


	for(int i = 1; i < argc; i++)
	{
		string flag = argv[i];
		string arg;
		
		if(i+1 < argc)
			arg = argv[i+1];
		else
			arg = "";

		if(flag.find("-f") != string::npos)
		{
			parseSMFFile(arg);
		}
		else if(flag.find("-j") != string::npos)
		{
			//viewport lower x bound
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for the window's upper y bound" << endl;
			viewLowerBoundX = atoi(arg.c_str());
		}
		else if(flag.find("-k") != string::npos)
		{
			//viewport lower y bound
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for the window's upper y bound" << endl;
			viewLowerBoundY = atoi(arg.c_str());
		}
		else if(flag.find("-o") != string::npos)
		{
			//viewport upper x bound
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for the window's upper y bound" << endl;
			viewUpperBoundX = atoi(arg.c_str());
		}
		else if(flag.find("-p") != string::npos)
		{
			//viewport upper y bound
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for the window's upper y bound" << endl;
			viewUpperBoundY = atoi(arg.c_str());
		}
		else if(flag.find("-x") != string::npos)
		{
			//prp x
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the prp x" << endl;
			prpX = atof(arg.c_str());
		}
		else if(flag.find("-y") != string::npos)
		{
			//prp y
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the prp y" << endl;
			prpY = atof(arg.c_str());
		}
		else if(flag.find("-z") != string::npos)
		{
			//prp z
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the prp z" << endl;
			prpZ = atof(arg.c_str());
		}
		else if(flag.find("-X") != string::npos)
		{
			//vrp x
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the vrp x" << endl;
			vrpX = atof(arg.c_str());
		}
		else if(flag.find("-Y") != string::npos)
		{
			//vrp y
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the vrp y" << endl;
			vrpY = atof(arg.c_str());
		}
		else if(flag.find("-Z") != string::npos)
		{
			//vrp z
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the vrp z" << endl;
			vrpZ = atof(arg.c_str());
		}
		else if(flag.find("-q") != string::npos)
		{
			//vpn x
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the vpn x" << endl;
			vpnX = atof(arg.c_str());
		}
		else if(flag.find("-r") != string::npos)
		{
			//vpn y
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the vpn y" << endl;
			vpnY = atof(arg.c_str());
		}
		else if(flag.find("-w") != string::npos)
		{
			//vpn z
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for the vpn z" << endl;
			vpnZ = atof(arg.c_str());
		}
		else if (flag.find("-Q") != string::npos)
		{
			//vup x
			if (!isFloat(arg.c_str()))
				cerr << "Invalid argument for the vup x" << endl;
			vupX = atof(arg.c_str());
		}
		else if (flag.find("-R") != string::npos)
		{
			//vup y
			if (!isFloat(arg.c_str()))
				cerr << "Invalid argument for the vup y" << endl;
			vupY = atof(arg.c_str());
		}
		else if (flag.find("-W") != string::npos)
		{
			//vup z
			if (!isFloat(arg.c_str()))
				cerr << "Invalid argument for the vup z" << endl;
			vupZ = atof(arg.c_str());
		}
		else if (flag.find("-u") != string::npos)
		{
			//uMin
			if (!isFloat(arg.c_str()))
				cerr << "Invalid argument for the uMin" << endl;
			uMin = atof(arg.c_str());
		}
		else if (flag.find("-v") != string::npos)
		{
			//vMin y
			if (!isFloat(arg.c_str()))
				cerr << "Invalid argument for the vMin" << endl;
			vMin = atof(arg.c_str());
		}
		else if (flag.find("-U") != string::npos)
		{
			//uMax z
			if (!isFloat(arg.c_str()))
				cerr << "Invalid argument for the uMax" << endl;
			uMax = atof(arg.c_str());
		}
		else if (flag.find("-V") != string::npos)
		{
			//vMax
			if (!isFloat(arg.c_str()))
				cerr << "Invalid argument for the vMax" << endl;
			vMax = atof(arg.c_str());
		}
		else if (flag.find("-P") != string::npos)
		{
			//perspective duh
			perspective = false;
		}
	}

	Vector3D* VPN = new Vector3D(vpnX, vpnY, vpnZ);
	Vector3D* VUP = new Vector3D(vupX, vupY, vupZ);
	Vector3D* VRR = VUP->cross(*VPN);
	Vector3D* VRP = new Vector3D(vrpX, vrpY, vrpZ);
	Vector3D* PRP = new Vector3D(prpX, prpY, prpZ);

	VPN->normalize();
	VUP->normalize();
	VRR->normalize();

	Matrix4D view = makeViewMatrix(VPN, VUP, VRR, VRP);

	Matrix4D projection = makeProjectionMatrix(PRP, uMax, uMin, vMax, vMin, front, back, perspective);

	convert(view, projection);
	if (shapes.size() == 0);
	clip(prpZ, front, back, perspective);
	if (shapes.size() == 0);
	projectOnto2D(perspective);
	if (shapes.size() == 0);

	//for(int i = 0; i < lines.size(); i++)
	//{
	//	cout << lines[i].point1.x << endl;
	//	cout << lines[i].point1.y << endl;
	//	cout << lines[i].point2.x << endl;
	//	cout << lines[i].point2.y << endl;
	//}

	//worldToScreen(lowerBoundX, lowerBoundY, upperBoundY);
	worldToViewport(-1, 1, -1, 1,
 					viewLowerBoundX, viewUpperBoundX, viewLowerBoundY, viewUpperBoundY); 	
	outputXPMFile(0,501,0,501);

	return 0;
}

