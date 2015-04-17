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

Shape *shapes[9];
static vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
static vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
static vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
static vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
static vec4 black = vec4(0.0, 0.0, 0.0, 1.0);

//--------------------------------------------------------------------------

void
init( void )
{
	// Initialization
	GLuint vao, buffer, program, location1, location2;

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
  
	// Create shapes	
	shapes[0] = new Square(0.0, -0.3, 0.6, GL_TRIANGLE_FAN, white);
	shapes[1] = new Triangle(0.0, 0.8, 0.2, GL_TRIANGLES);
	shapes[2] = new Circle(0.6, 0.7, 0.15, GL_TRIANGLE_FAN);
	shapes[3] = new Circle(-0.6, 0.7, 1.0, 0.6, 0.15, GL_TRIANGLE_FAN);
	shapes[4] = new Square(0.0, -0.3, 0.5, GL_TRIANGLE_FAN, black);
	shapes[5] = new Square(0.0, -0.3, 0.4, GL_TRIANGLE_FAN, white);
	shapes[6] = new Square(0.0, -0.3, 0.3, GL_TRIANGLE_FAN, black);
	shapes[7] = new Square(0.0, -0.3, 0.2, GL_TRIANGLE_FAN, white);
	shapes[8] = new Square(0.0, -0.3, 0.1, GL_TRIANGLE_FAN, black);

    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

	// Copy the static vectors into arrays for the buffer
	vec2 *points = &Shape::coords[0];
	vec4 *cols = &Shape::colors[0];

	// Load array into buffer
    glBufferData( GL_ARRAY_BUFFER, 
					(Shape::coords.size() * sizeof(vec2)) + (Shape::colors.size() * sizeof(vec4)), 
					NULL, 
					GL_STATIC_DRAW );
	glBufferSubData(GL_ARRAY_BUFFER, 
					0,
					Shape::coords.size() * sizeof(vec2), 
					points ); 
	glBufferSubData(GL_ARRAY_BUFFER, 
					Shape::coords.size() * sizeof(vec2), 
					Shape::colors.size() * sizeof(vec4), 
					cols ); 

    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
	glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
	location1 = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( location1 );
    glVertexAttribPointer( location1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // Initialize the fragment color attribute from the fragment shader
	location2 = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( location2 );
    glVertexAttribPointer( location2, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(Shape::coords.size() * sizeof(vec2)) );

    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

	int start = 0;
	for(int i = 0; i < 9; i++)
	{
    	glDrawArrays( shapes[i]->mode, start, shapes[i]->numPoints);  // draw the points
		cout << "start: " << start << "  end: " << start + shapes[i]->numPoints - 1 << endl;
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

