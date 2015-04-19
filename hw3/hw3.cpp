// Sarah Kushner
// CS 432
// Assignment 3

#include "Angel.h"
#include "Shape.h"
#include "Circle.h"
#include "Triangle.h"
#include "Square.h"
#include <iostream>

using namespace std;

// Shape arrays
Shape *shapesMain[6];
Shape *shapesSub[1];
Shape *shapesSecond[2];

// Point and Color arrays
vec2 *points;
vec4 *cols; 

// Animation
//static bool animated = true;
static GLfloat currentAngleOfRotation = 0.0;

// Basic Colors
static vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
static vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
static vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
static vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
static vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
static vec4 purple = vec4(0.5, 0.12, 0.67, 1.0);
static vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);
static vec4 orange = vec4(0.89, 0.325, 0.0, 1.0);

// Windows
int mainWin, subWin, secondWin;


//--------------------------------------------------------------------------

void loadBuffer( void )
{
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
}

//--------------------------------------------------------------------------

void recolorShapes( vec4 color, int start, int end )
{
	for(int i = start; i < end; i++)
	{
		Shape::colors.at(i) = color;
	}
	loadBuffer();
	glutPostRedisplay();
}

//--------------------------------------------------------------------------

void setupGeometry( void )
{
	// Create shapes	
	shapesMain[0] = new Square(0.0, -0.3, 0.6, GL_TRIANGLE_FAN, white);
	shapesMain[1] = new Square(0.0, -0.3, 0.5, GL_TRIANGLE_FAN, black);
	shapesMain[2] = new Square(0.0, -0.3, 0.4, GL_TRIANGLE_FAN, white);
	shapesMain[3] = new Square(0.0, -0.3, 0.3, GL_TRIANGLE_FAN, black);
	shapesMain[4] = new Square(0.0, -0.3, 0.2, GL_TRIANGLE_FAN, white);
	shapesMain[5] = new Square(0.0, -0.3, 0.1, GL_TRIANGLE_FAN, black);

	shapesSub[0] = new Circle(0.0, 0.0, 1.0, 0.8, 0.7, GL_TRIANGLE_FAN, red);

	shapesSecond[0] = new Circle(-0.5, -0.5, 0.3, GL_TRIANGLE_FAN, purple);
	shapesSecond[1] = new Triangle(0.5, 0.5, 0.3, GL_TRIANGLE_FAN, purple);

	// Copy the static vectors into arrays for the buffer
	points = &Shape::coords[0];
	cols = &Shape::colors[0];
}


//--------------------------------------------------------------------------


void init( vec4 bgColor )
{
	// Initialization
	GLuint vao, buffer, program, location1, location2;

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

	// Load array into buffer
	loadBuffer();

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

	glClearColor( bgColor[0], bgColor[1], bgColor[2], bgColor[3] );
	//glutPostRedisplay();

}

//----------------------------------------------------------------------------

void displayMain( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glRotatef( currentAngleOfRotation, 0.0, 0.0, 1.0 );

	int start = 0;
	for(int i = 0; i < 6; i++)
	{
    	glDrawArrays( shapesMain[i]->mode, start, shapesMain[i]->numPoints);  // draw the points
		start += shapesMain[i]->numPoints;
	}

	//Shape::coords.clear();
	//Shape::colors.clear();

	glutSwapBuffers();
}

void displaySub( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

	int start = 24;
	for(int i = 0; i < 1; i++)
	{
    	glDrawArrays( shapesSub[i]->mode, start, shapesSub[i]->numPoints);  // draw the points
		start += shapesSub[i]->numPoints;
	}
	
	//Shape::coords.clear();
	//Shape::colors.clear();

	glutSwapBuffers();
}

void displaySecond( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

	int start = 324;
	for(int i = 0; i < 2; i++)
	{
    	glDrawArrays( shapesSecond[i]->mode, start, shapesSecond[i]->numPoints);  // draw the points
		start += shapesSecond[i]->numPoints;
	}

	//Shape::coords.clear();
	//Shape::colors.clear();

	glutSwapBuffers();
}


//----------------------------------------------------------------------------

void idleMain( void )
{
	glTranslatef( 0.0, -0.3, 0.0 );
	glRotatef( 360, 0, 0, 1 );
	glTranslatef( 0.0, 0.3, 0.0 );
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

void secondKeyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
	case 'r':
		recolorShapes( red, 324, Shape::colors.size() );
		break;
	case 'g':
		recolorShapes( green, 324, Shape::colors.size() );
		break;
	case 'b':
		recolorShapes( blue, 324, Shape::colors.size() );
		break;
	case 'y':
		recolorShapes( yellow, 324, Shape::colors.size() );
		break;
	case 'o':
		recolorShapes( orange, 324, Shape::colors.size() );
		break;
	case 'p':
		recolorShapes( purple, 324, Shape::colors.size() );
		break;
	case 'w':
		recolorShapes( white, 324, Shape::colors.size() );
		break;

    }
}

//----------------------------------------------------------------------------

void colorMenu( int id )
{
	switch( id ) {
		case 1: 
			glClearColor( black[0], black[1], black[2], black[3] );
			break;
		case 2:
		   	glClearColor( white[0], white[1], white[2], white[3] );
			break;
		case 3:
		   	glClearColor( red[0], red[1], red[2], red[3] );
			break;
		case 4:
			glClearColor( green[0], green[1], green[2], green[3] );
			break;
		case 5:
		   	glClearColor( blue[0], blue[1], blue[2], blue[3] );
			break;
		case 6:
		   	glClearColor( yellow[0], yellow[1], yellow[2], yellow[3] );
			break;
		case 7: 
			glClearColor( purple[0], purple[1], purple[2], purple[3] );
			break;
		case 8:
		   	glClearColor( orange[0], orange[1], orange[2], orange[3] );
			break;
	}

	glutPostRedisplay();
}

void mainMenu( int id )
{
	switch( id ) {
		case 1: 
			break;
		case 2:
			break;
		case 3:
			recolorShapes( white, 0, 3 );
			recolorShapes( white, 8, 11 );
			recolorShapes( white, 16, 19 );
			break;
		case 4:
			recolorShapes( red, 0, 3 );
			recolorShapes( red, 8, 11 );
			recolorShapes( red, 16, 19 );
			break;
		case 5:
			recolorShapes( green, 0, 3 );
			recolorShapes( green, 8, 11 );
			recolorShapes( green, 16, 19 );
			break;
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	setupGeometry();

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "Shapes, man." );
    glewExperimental=GL_TRUE; 
    glewInit(); 
	int submenu = glutCreateMenu( mainMenu ); //submenu
	glutAddMenuEntry( "White", 3 );
	glutAddMenuEntry( "Red", 4 );
	glutAddMenuEntry( "Green", 5 );

	glutCreateMenu( mainMenu ); // Set up menu
	glutAddMenuEntry ( "Stop Animation", 1 );
	glutAddMenuEntry( "Start Animation", 2 );
	glutAddSubMenu( "Square Colors", submenu ); // add submenu
	glutAttachMenu( GLUT_RIGHT_BUTTON );

    init( black );

	glutIdleFunc( idleMain );
    glutDisplayFunc( displayMain );
    glutKeyboardFunc( keyboard );

	// Sub Window
	subWin = glutCreateSubWindow( mainWin, 0, 0, 200, 100 );
	glutDisplayFunc( displaySub );
	glutCreateMenu( colorMenu ); // Set up menu
	glutAddMenuEntry ( "Black", 1 );
	glutAddMenuEntry( "White", 2 );
	glutAddMenuEntry( "Red", 3 );
	glutAddMenuEntry( "Green", 4 );
	glutAddMenuEntry( "Blue", 5 );
	glutAddMenuEntry( "Yellow", 6 );
	glutAddMenuEntry( "Purple", 7 );
	glutAddMenuEntry( "Orange", 8 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );
    init( white );


	// Second Window
    secondWin = glutCreateWindow( "Window 2" );
    glutInitWindowSize( 500, 500 );
    glewInit();    
    init( white );

    glutDisplayFunc( displaySecond );
    glutKeyboardFunc( secondKeyboard );

	// Run
    glutMainLoop();
    return 0;
}

