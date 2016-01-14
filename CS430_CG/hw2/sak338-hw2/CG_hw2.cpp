// Sarah Kushner
// CS 430
// HW 2

#define _USE_MATH_DEFINES

#include "Shape.h"
#include "Line.h"
#include "Polygon.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace std;

vector<Shape*> shapes;

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

void parsePSFile(string name)
{
	ifstream infile;

	string filename = "./";
	string line;

	filename += name;
	infile.open(filename.c_str());
	
	bool read = false;

	deque<int> linePoints;
	Polygon* currentPolygon = new Polygon();

	if (infile)
	{
		while (getline( infile, line )) 
		{
			if (line.find("%%%BEGIN") != string::npos)
			{
				read = true;
			}
			else if (line.find("%%%END") != string::npos)
			{
				break;
			}
			else if (read)
			{
				if (line.find("Line") != string::npos)
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
					shapes.push_back(l);
					linePoints.clear();
				}
				else if (line.find("moveto") != string::npos)
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
					currentPolygon = new Polygon();
				}
				else if (line.find("lineto") != string::npos)
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
					currentPolygon->addVertex(linePoints[0], linePoints[1]);

					linePoints.pop_front();
					linePoints.pop_front();
				}
				else if (line.find("stroke") != string::npos)
				{
					currentPolygon->addVertex(linePoints[0], linePoints[1]);
					linePoints.pop_front();
					linePoints.pop_front();
					shapes.push_back(currentPolygon);
				}
			}
		}
		infile.close();
	}
}

void transform(float scaleFactor, float radians, int translateX, int translateY)
{
	for (Shape* s : shapes)
		s->transform(scaleFactor, radians, translateX, translateY);
}

void clip(int lowerBoundX, int lowerBoundY, int upperBoundX, int upperBoundY)
{
	for (int i = 0; i < shapes.size(); i++)
	{
		Shape* s = shapes[i];
		bool success = s->clip(lowerBoundX, lowerBoundY, upperBoundX, upperBoundY);
		if (!success)
		{
			shapes.erase(shapes.begin() + i);
			i--;
		}
	}
}

void worldToScreen(int lowerBoundX, int lowerBoundY, int upperBoundY)
{
	for (Shape* s : shapes)
		s->worldToScreen(lowerBoundX, lowerBoundY, upperBoundY);
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

	for (Shape* s : shapes)
		s->draw(screen);

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
			if ( !isFloat(arg.c_str()) )
				cerr << "Invalid argument for scaling" << endl;
			scaleFactor = atof(arg.c_str());
		}
		else if(flag.find("-r") != string::npos)
		{
			//rotate
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for rotation" << endl;
			rotateDegree = atoi(arg.c_str());
			radians = rotateDegree * M_PI / 180;

		}
		else if(flag.find("-m") != string::npos)
		{
			//translate in the x direction
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for translation in the x direction" << endl;
			translateX = atoi(arg.c_str());
		}
		else if(flag.find("-n") != string::npos)
		{
			//translate in the y direction
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for translation in the y direction" << endl;
			translateY = atoi(arg.c_str());
		}
		else if(flag.find("-a") != string::npos)
		{
			//window lower x bound
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for the window's lower x bound" << endl;
			lowerBoundX = atoi(arg.c_str());
		}
		else if(flag.find("-b") != string::npos)
		{
			//window lower y bound
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for the window's lower y bound" << endl;
			lowerBoundY = atoi(arg.c_str());
		}
		else if(flag.find("-c") != string::npos)
		{
			//window upper x bound
			if ( !isNumber(arg.c_str()) )
				cerr << "Invalid argument for the window's upper x bound" << endl;
			upperBoundX = atoi(arg.c_str());
		}
		else if(flag.find("-d") != string::npos)
		{
			//window upper y bound
			if ( !isNumber(arg.c_str()) )
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

	worldToScreen(lowerBoundX, lowerBoundY, upperBoundY);
	outputXPMFile(lowerBoundX, lowerBoundY, upperBoundX, upperBoundY);

	return 0;
}

