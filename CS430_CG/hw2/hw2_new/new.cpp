#include "Polygon.h"

using namespace std;

int main()
{
	Polygon *p = new Polygon;
	p->addVertex(-1,-5);
	p->addVertex(5,5);
	p->addVertex(2,7);
	p->addVertex(-1,-5);

	p->clip(0,0,10,10);

	return 0;
}
