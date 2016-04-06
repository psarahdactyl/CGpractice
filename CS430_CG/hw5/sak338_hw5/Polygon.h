#ifndef POLYGON_H
#define POLYGON_H

#include "Vector3D.h"
//#include "Vector4D.h"
#include "Matrix4D.h"
#include "Shape.h"

class Polygon : public Shape
{
	public:
		Polygon();
		void addVertex(Vector4D* v);
		void addVertex(int x, int y, int z);
		virtual bool clip(float prpZ, float front, float back, bool perspective);
		virtual void draw(float ** zBuff, char ** screen, int height, int width);
		void convert(Matrix4D view, Matrix4D projection);
		void project(bool perspective);
		virtual void fill(float ** zBuff, char ** screen, int height, int width, float zMin, float zMax);
	private:
		//bool SutherlandHodgman(int minX, int minY, int maxX, int maxY);
};


#endif

