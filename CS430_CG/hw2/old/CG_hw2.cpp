// Sarah Kushner
// CS 430
// HW 2

#include "Line.h"
#include "Vector.h"
#include "Matrix.h"
#include "Transformation.h"
#include "Clipping.h"
#include "Drawing.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace std;

vector<Line> lines;
clipper clippingAlg;

void parsePSFile(string name)
{
	ifstream infile;

	string filename = "./";
	string line;

	filename += name;
	infile.open(filename.c_str());
	
	bool read = false;

	deque<int> linePoints;

	if(infile)
	{
		while (getline( infile, line )) 
		{
			if(line.find("%%%BEGIN") != string::npos)
			{
				read = true;
			}
			else if(line.find("%%%END") != string::npos)
			{
				break;
			}
			else if(read)
			{
				if(line.find("Line") != string::npos)
				{
					clippingAlg = CohenSutherland;
					size_t pos = 0;
					string delimiter = " ";
					string point;

					while ((pos = line.find(delimiter)) != string::npos) 
					{
						point = line.substr(0, pos);
						linePoints.push_back( atoi(point.c_str()) );
						line.erase(0, pos + delimiter.length());
					}
					Line *l = new Line(linePoints[0], linePoints[1], linePoints[2], linePoints[3]);
					lines.push_back(*l);
					linePoints.clear();
				}
				else if(line.find("moveto") != string::npos)
				{
					clippingAlg = SutherlandHodgman;
					size_t pos = 0;
					string delimiter = " ";
					string point;

					while ((pos = line.find(delimiter)) != string::npos) 
					{
						point = line.substr(0, pos);
						linePoints.push_back( atoi(point.c_str()) );
						line.erase(0, pos + delimiter.length());
					}

				}
				else if(line.find("lineto") != string::npos)
				{
					size_t pos = 0;
					string delimiter = " ";
					string point;

					while ((pos = line.find(delimiter)) != string::npos) 
					{
						point = line.substr(0, pos);
						linePoints.push_back( atoi(point.c_str()) );
						line.erase(0, pos + delimiter.length());
					}
					Line *l = new Line(linePoints[0], linePoints[1], linePoints[2], linePoints[3]);
					lines.push_back(*l);

					linePoints.pop_front();
					linePoints.pop_front();
				}
				else if(line.find("stroke") != string::npos)
				{
					linePoints.pop_front();
					linePoints.pop_front();
				}
			}
		}
		infile.close();
	}

}

void transform(float scaleFactor, float radians, int translateX, int translateY)
{
	//scale
	Matrix3D s = Transformation::createScaleMat(scaleFactor);
	//rotate
	Matrix3D r = Transformation::createRotateCCWMat(radians);
	//translate
	Matrix3D t = Transformation::createTranslate2DMat(translateX, translateY);

	Matrix3D m = t*r*s;

	for(int i = 0; i < lines.size(); i++)
	{
		lines[i].point1 = m * lines[i].point1;
		lines[i].point2 = m * lines[i].point2;
	}
}

void clip(int lowerBoundX, int lowerBoundY, int upperBoundX, int upperBoundY)
{
		//clip
		switch(clippingAlg)
		{
			case CohenSutherland:
			{
				for(int i = 0; i < lines.size(); i++)
				{
					Line *l = Clipping::CohenSutherland(lines[i], lowerBoundX, lowerBoundY, upperBoundX, upperBoundY);
					if(l != NULL)
						lines[i] = *l;
					else
					{
						lines.erase(lines.begin()+i);
						i--;
					}
					
				}
				break;
			}
			case SutherlandHodgman:
			{
			
				break;	
			}
		}
}

void transformToScreen(int lowerBoundX, int lowerBoundY, int upperBoundY)
{
	int screenHeight = upperBoundY - lowerBoundY;

	for(int i = 0; i < lines.size(); i++)
	{

		lines[i].point1.x = lines[i].point1.x - lowerBoundX;
		lines[i].point1.y = lines[i].point1.y - lowerBoundY;
		lines[i].point1 = Transformation::translate2D(lines[i].point1, 0, -1 * screenHeight);
		lines[i].point1.y = -1 * lines[i].point1.y;

		lines[i].point2.x = lines[i].point2.x - lowerBoundX;
		lines[i].point2.y = lines[i].point2.y - lowerBoundY;
		lines[i].point2 = Transformation::translate2D(lines[i].point2, 0, -1 * screenHeight);
		lines[i].point2.y = -1 * lines[i].point2.y;
	}
	
}

void outputXPMFile(int lowerBoundX, int lowerBoundY, int upperBoundX, int upperBoundY)
{

 	int width = upperBoundX - lowerBoundX + 1;
	int height = upperBoundY - lowerBoundY + 1;

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

	for(int i = 0; i < lines.size(); i++)
	{
		screen = Drawing::DDA(lines[i], screen);
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
	float scaleFactor = 1.0;
	int rotateDegree = 0;
	float radians = 0;
	int translateX = 0;
	int translateY = 0;
	int lowerBoundX = 0;
	int lowerBoundY = 0;
	int upperBoundX = 499;
	int upperBoundY = 499;

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
			parsePSFile(arg);
		}
		else if(flag.find("-s") != string::npos)
		{
			//scale
			if ( !( isdigit(arg[0]) || isdigit(arg[0]* -1) ) )
				cerr << "Invalid argument for scaling" << endl;
			scaleFactor = atof(arg.c_str());
		}
		else if(flag.find("-r") != string::npos)
		{
			//rotate
			if ( !( isdigit(arg[0]) || isdigit(arg[0]* -1) ) )
				cerr << "Invalid argument for rotation" << endl;
			rotateDegree = atoi(arg.c_str());
			radians = rotateDegree * M_PI / 180;

		}
		else if(flag.find("-m") != string::npos)
		{
			//translate in the x direction
			if ( !( isdigit(arg[0]) || isdigit(arg[0]* -1) ) )
				cerr << "Invalid argument for translation in the x direction" << endl;
			translateX = atoi(arg.c_str());
		}
		else if(flag.find("-n") != string::npos)
		{
			//translate in the y direction
			if ( !( isdigit(arg[0]) || isdigit(arg[0]* -1) ) )
				cerr << "Invalid argument for translation in the y direction" << endl;
			translateY = atoi(arg.c_str());
		}
		else if(flag.find("-a") != string::npos)
		{
			//window lower x bound
			if ( !( isdigit(arg[0]) || isdigit(arg[0]* -1) ) )
				cerr << "Invalid argument for the window's lower x bound" << endl;
			lowerBoundX = atoi(arg.c_str());
		}
		else if(flag.find("-b") != string::npos)
		{
			//window lower y bound
			if ( !( isdigit(arg[0]) || isdigit(arg[0]* -1) ) )
				cerr << "Invalid argument for the window's lower y bound" << endl;
			lowerBoundY = atoi(arg.c_str());
		}
		else if(flag.find("-c") != string::npos)
		{
			//window upper x bound
			if ( !( isdigit(arg[0]) || isdigit(arg[0]* -1) ) )
				cerr << "Invalid argument for the window's upper x bound" << endl;
			upperBoundX = atoi(arg.c_str());
		}
		else if(flag.find("-d") != string::npos)
		{
			//window upper y bound
			if ( !( isdigit(arg[0]) || isdigit(arg[0]* -1) ) )
				cerr << "Invalid argument for the window's upper y bound" << endl;
			upperBoundY = atoi(arg.c_str());
		}

	}

	int midX = (upperBoundX - lowerBoundX) / 2;
	int midY = (upperBoundY - lowerBoundY) / 2;

	transform(scaleFactor, radians, translateX, translateY);
	clip(lowerBoundX, lowerBoundY, upperBoundX, upperBoundY);

	//for(int i = 0; i < lines.size(); i++)
	//{
	//	cout << lines[i].point1.x << endl;
	//	cout << lines[i].point1.y << endl;
	//	cout << lines[i].point2.x << endl;
	//	cout << lines[i].point2.y << endl;
	//}

	transformToScreen(lowerBoundX, lowerBoundY, upperBoundY);
	outputXPMFile(lowerBoundX, lowerBoundY, upperBoundX, upperBoundY);

	return 0;
}

