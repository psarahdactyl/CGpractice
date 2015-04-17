// Sarah Kushner
// CS 432
// Assignment 2
// Recreates picture given on homework 2 page.

#include "Angel.h"
#include "Shape.h"
#include "Circle.h"
#include <iostream>

using namespace std;

Shape *shapes[1];

//--------------------------------------------------------------------------

void
init( void )
{
	// Create shapes	
	shapes[0] = new Circle(0.0, 0.0, 1, 100, GL_TRIANGLES);
	cout << shapes[0] << endl;

    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
	for(int i = 0; i < 1; i++)
	{
		shapes[i]->draw();
	}

    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 500, 500 );

    glutCreateWindow( "Shapes, man." );
    glewExperimental=GL_TRUE; 
    glewInit();    
    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}

