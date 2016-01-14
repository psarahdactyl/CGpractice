// Sarah Kushner
// CS 430
// Honors Assignment

#include "Angel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <stdexcept>
#include <math.h>

using namespace std;

typedef GLfloat point3[3];

// Globals, I'm so sorry

// Point and Color arrays
vec2* points;
vec2* lines;

vector<vec2> controlPoints;
vector<vec2> linePoints;

int pointsSize;
int linesSize;

// VAO stuff
GLuint vPosition;

GLfloat aspect;       // Viewport aspect ratio

// Control Point size
float CPsize = 5.0;

// Windows
int mainWin;

//--------------------------------------------------------------------------


vec2* deCasteljau(int r, int i, double t) 
{ 
	if(r == 0) 
		return points+i;

	vec2* p1 = deCasteljau(r - 1, i, t);
	vec2* p2 = deCasteljau(r - 1, i + 1, t);

	return new vec2(((1 - t) * p1->x + t * p2->x),((1 - t) * p1->y + t * p2->y));
}


void createCurve(vec2* points, int pointsSize)
{
	vec2* tmp;
	for (double t = 0; t <= 1; t += 0.01) 
	{
		tmp = deCasteljau(pointsSize-1, 0, t);
		linePoints.push_back( vec2(tmp->x, tmp->y) );
		linesSize++;
	}

}

//--------------------------------------------------------------------------

void loadBuffer( void )
{
	// Load array into buffer
	//glBufferData( GL_ARRAY_BUFFER, sizeof(vec2)*pointsSize, points, GL_STATIC_DRAW );

	glBufferData( GL_ARRAY_BUFFER, sizeof(vec2)*pointsSize +
							 sizeof(vec2)*linesSize, NULL, GL_STATIC_DRAW );

	glBufferSubData( GL_ARRAY_BUFFER, 0,
							  sizeof(vec2)*pointsSize, points );

	glBufferSubData( GL_ARRAY_BUFFER, sizeof(*points)*pointsSize,
							   sizeof(vec2)*linesSize, lines );

}

//--------------------------------------------------------------------------

void init( )
{
	// Initialization
	GLuint vao, buffer;

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

	// Load array into buffer
	loadBuffer();

    glBindVertexArray( vao );
	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "vshdr.glsl", "fshdr.glsl" );
	glUseProgram( program );

	// Set up vertex arrays
	vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
					    BUFFER_OFFSET(0) );

	glClearColor( 1.0, 1.0, 1.0, 1.0 );

}

//----------------------------------------------------------------------------

void addPoint(int sx, int sy) 
{
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble x, y, z;
	gluUnProject(sx, viewport[1] + viewport[3] - sy, 0, modelMatrix, projMatrix, viewport, &x, &y, &z);

	GLfloat nearv[3], farv[3];
	nearv[0] = x;
	nearv[1] = y;
	nearv[2] = z;

	gluUnProject(sx, viewport[1] + viewport[3] - sy, 1, modelMatrix, projMatrix, viewport, &x, &y, &z);

	farv[0] = x;
	farv[1] = y;
	farv[2] = z;

	GLfloat t = (nearv[2] - z) / (nearv[2] - farv[2]);

	GLfloat new_x = nearv[0] + (farv[0] - nearv[0]) * t;
	GLfloat new_y = nearv[1] + (farv[1] - nearv[1]) * t;

	//cout << new_x << ", " << new_y << endl;

	controlPoints.push_back( vec2(new_x, new_y) );
}

//----------------------------------------------------------------------------

void displayMain( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

	glDrawArrays( GL_POINTS, 0, pointsSize);
	glDrawArrays( GL_LINE_STRIP, pointsSize, linesSize);
	glPointSize( CPsize );
    glFlush();

}

//----------------------------------------------------------------------------

void idleMain( void )
{
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void reshapeMain( int width, int height )
{
	glViewport( 0, 0, width, height );
	aspect = GLfloat(width)/height;
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 'q':
        exit( EXIT_SUCCESS );
        break;
  	case 'c':
		linePoints.clear();
		controlPoints.clear();
		linesSize = 0;
		pointsSize = 0;
		loadBuffer();
		break;
  	case 's':
		CPsize += 1.0;
		break;
  	case 'S':
		CPsize -= 1.0;
		break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void mainMenu( int id )
{
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void mouseMain( int button, int state, int mouse_x, int mouse_y )
{

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		linesSize = 0;
		linePoints.clear();

		addPoint(mouse_x, mouse_y);

		pointsSize++;
		points = &controlPoints[0];

		createCurve(points, pointsSize);
		lines = &linePoints[0];

		loadBuffer();
		
	}

}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{

	cout << endl;
	cout << "---------------------" << endl;
	cout << "INSTRUCTIONS FOR KEYS" << endl;
	cout << "---------------------" << endl;
	cout << endl;
	cout << "'s (lowercase)' - increases size of all control points " << endl;
	cout << "'S (uppercase)' - decreases size of all control points " << endl;
	cout << "'c' -  clears the curve and all points" << endl;
	cout << endl;
	cout << "'q' -  quits the program" << endl;
	cout << endl;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "Dude, it's a Bezier playground" );
    glewExperimental=GL_TRUE; 
    glewInit(); 

    init();

	glutCreateMenu( mainMenu ); // Set up menu
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutIdleFunc( idleMain );
    glutDisplayFunc( displayMain );
    glutReshapeFunc( reshapeMain );
    glutKeyboardFunc( keyboard );
	glutMouseFunc(mouseMain);

	glEnable( GL_LINE_SMOOTH );

	// Run
    glutMainLoop();
    return 0;
}

