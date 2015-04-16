// Sarah Kushner
// CS 432
// Assignment 2
// Recreates picture given on homework 2 page.

#include "Angel.h"
#include "Shape.h"
#include "Circle.h"
#include "Triangle.h"
#include "Square.h"
#include <iostream>

using namespace std;

Shape *shapes[3];

//--------------------------------------------------------------------------

void
init( void )
{
	// Initialization
	GLuint vao, buffer, program, location;

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
  
	// Create shapes	
	shapes[0] = new Circle(0.5, 0.5, .2, GL_TRIANGLE_FAN);
	shapes[1] = new Triangle(0.0, 0.8, .2, GL_TRIANGLES);
	shapes[2] = new Square(0.0, -0.25, .2, GL_TRIANGLES);


    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

	vec2 *points = &Shape::coords[0];

    glBufferData( GL_ARRAY_BUFFER, (Shape::coords.size() * sizeof(Shape::coords[0])), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
	location = glGetAttribLocation( program, "vPosition" );
	glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    glEnableVertexAttribArray( location );
    glVertexAttribPointer( location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

	int start = 0;
	for(int i = 0; i < 2; i++)
	{
    	glDrawArrays( shapes[i]->mode, start, start + shapes[i]->numPoints - 1);  // draw the points
		//cout << "start: " << start << "  end: " << start + shapes[i]->numPoints - 1 << endl;
		start += shapes[i]->numPoints;
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

