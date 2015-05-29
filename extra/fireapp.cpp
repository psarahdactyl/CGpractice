// Sarah Kushner
// CS 432
// Assignment 4

#include "Angel.h"
#include <iostream>

using namespace std;

// Point and Color arrays
vec4 points[36];
vec4 colors[36]; 

static unsigned int NumVertices;

// Transformation bools for each
bool scaleEnabled = false;
bool rotateEnabled = false;
bool translateEnabled = false;
// Matrix
mat4 s;
mat4 rx;
mat4 ry;
mat4 rz;
mat4 t;

// Shader variables
GLuint scaleLoc;
GLuint rotateLocX;
GLuint rotateLocY;
GLuint rotateLocZ;
GLuint translateLoc;

GLfloat vector[3];
GLfloat matrix[16];

// x,y,z values, delta, theta
GLfloat xscale = 1.0;
GLfloat yscale = 1.0;
GLfloat zscale = 1.0;

GLfloat xrotate = 1.0;
GLfloat yrotate = 1.0;
GLfloat zrotate = 1.0;

GLfloat xtranslate = 0.0;
GLfloat ytranslate = 0.0;
GLfloat ztranslate = 0.0;

GLfloat xtheta = 0.0;
GLfloat ytheta = 0.0;
GLfloat ztheta = 0.0;

GLfloat scaledelta = 0.25;
GLfloat thetadelta = M_PI/4;
GLfloat translatedelta = 0.25;


// Vertices of a unit cube centered at origin
// sides aligned with axes
static vec4 vertices[8] = {
	vec4( -0.5, -0.5, 0.5, 1.0 ),
	vec4( -0.5, 0.5, 0.5, 1.0 ),
	vec4( 0.5, 0.5, 0.5, 1.0 ),
	vec4( 0.5, -0.5, 0.5, 1.0 ),
	vec4( -0.5, -0.5, -0.5, 1.0 ),
	vec4( -0.5, 0.5, -0.5, 1.0 ),
	vec4( 0.5, 0.5, -0.5, 1.0 ),
	vec4( 0.5, -0.5, -0.5, 1.0 )
}; 

vec4 vertices_original[8] = vertices;

// RGBA colors
static vec4 vertex_colors[8] = {
	vec4( 0.0, 0.0, 0.0, 1.0 ), // black
	vec4( 1.0, 0.0, 0.0, 1.0 ), // red
	vec4( 1.0, 1.0, 0.0, 1.0 ), // yellow
	vec4( 0.0, 1.0, 0.0, 1.0 ), // green
	vec4( 0.0, 0.0, 1.0, 1.0 ), // blue
	vec4( 1.0, 0.0, 1.0, 1.0 ), // magenta
	vec4( 1.0, 1.0, 1.0, 1.0 ), // white
	vec4( 0.0, 1.0, 1.0, 1.0 ) // cyan
};

// Windows
int mainWin;


//--------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
// to the vertices
int Index = 0;
void quad( int a, int b, int c, int d )
{
	colors[Index] = vertex_colors[a]; 
	points[Index] = vertices[a];
	Index++;
	colors[Index] = vertex_colors[b]; 
	points[Index] = vertices[b]; 
	Index++;
	colors[Index] = vertex_colors[c]; 
	points[Index] = vertices[c]; 
	Index++;
	colors[Index] = vertex_colors[a]; 
	points[Index] = vertices[a]; 
	Index++;
	colors[Index] = vertex_colors[c]; 
	points[Index] = vertices[c]; 
	Index++;
	colors[Index] = vertex_colors[d]; 
	points[Index] = vertices[d]; 
	Index++;
}

//--------------------------------------------------------------------------

void loadBuffer( void )
{
	// Load array into buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) +
							 sizeof(colors), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,
							  sizeof(points), points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points),
							   sizeof(colors), colors );


}

//--------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
	Index = 0;
	NumVertices = 36;
	quad( 1, 0, 3, 2 );
	quad( 2, 3, 7, 6 );
	quad( 3, 0, 4, 7 );
	quad( 6, 5, 1, 2 );
	quad( 4, 5, 6, 7 );
	quad( 5, 4, 0, 1 );
} 

//--------------------------------------------------------------------------

void init( )
{
	colorcube();

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

	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "vshdrcube.glsl", "fshdrcube.glsl" );
	glUseProgram( program );

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
					    BUFFER_OFFSET(0) );

	GLuint vColor = glGetAttribLocation( program, "vColor" );
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(points)) );

	scaleLoc = glGetUniformLocation( program, "scale" );
	rotateLocX = glGetUniformLocation( program, "rotatex" );
	rotateLocY = glGetUniformLocation( program, "rotatey" );
	rotateLocZ = glGetUniformLocation( program, "rotatez" );
	translateLoc = glGetUniformLocation( program, "translate" );

	glClearColor( 1.0, 1.0, 1.0, 1.0 );

}

//----------------------------------------------------------------------------

void displayMain( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

	glUniformMatrix4fv( scaleLoc, 1, GL_FALSE, &s[0][0] );
	glUniformMatrix4fv( rotateLocX, 1, GL_FALSE, &rx[0][0] );
	glUniformMatrix4fv( rotateLocY, 1, GL_FALSE, &ry[0][0] );
	glUniformMatrix4fv( rotateLocZ, 1, GL_FALSE, &rz[0][0] );
	glUniformMatrix4fv( translateLoc, 1, GL_FALSE, &t[0][0] );

	glDrawArrays( GL_TRIANGLES, 0, NumVertices );

	glutSwapBuffers();
}

void idleMain( void )
{
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 'q':
        exit( EXIT_SUCCESS );
        break;
    case 'w':
		if(scaleEnabled)
			xscale += scaledelta;
		else if(rotateEnabled)
			xtheta += thetadelta;
		else if(translateEnabled)
			xtranslate += translatedelta;
        break;
    case 'e':
		if(scaleEnabled)
			yscale += scaledelta;
		else if(rotateEnabled)
			ytheta += thetadelta;
		else if(translateEnabled)
			ytranslate += translatedelta;
        break;
    case 'r':
		if(scaleEnabled)
			zscale += scaledelta;
		else if(rotateEnabled)
			ztheta += thetadelta;
		else if(translateEnabled)
			ztranslate += translatedelta;
        break;
    case 'a':
		if(scaleEnabled)
			xscale -= scaledelta;
		else if(rotateEnabled)
			xtheta -= thetadelta;
		else if(translateEnabled)
			xtranslate -= translatedelta;
        break;
    case 's':
		if(scaleEnabled)
			yscale -= scaledelta;
		else if(rotateEnabled)
			ytheta -= thetadelta;
		else if(translateEnabled)
			ytranslate -= translatedelta;
        break;
    case 'd':
		if(scaleEnabled)
			zscale -= scaledelta;
		else if(rotateEnabled)
			ztheta -= thetadelta;
		else if(translateEnabled)
			ztranslate -= translatedelta;
        break;
    case 'z':
		s = identity();
		rx = identity();
		ry = identity();
		rz = identity();
		t = identity();

		xscale = 1.0;
		yscale = 1.0;
		zscale = 1.0;
		
		xrotate = 1.0;
		yrotate = 1.0;
		zrotate = 1.0;
		
		xtranslate = 0.0;
		ytranslate = 0.0;
		ztranslate = 0.0;
		
		xtheta = 0.0;
		ytheta = 0.0;
		ztheta = 0.0;
        break;
    case 'n':
		if(scaleEnabled)
			scaledelta -= scaledelta;
		else if(rotateEnabled)
			thetadelta -= (M_PI/4);
		else if(translateEnabled)
			translatedelta -= translatedelta;
        break;
    case 'm':
		if(scaleEnabled)
			scaledelta += scaledelta;
		else if(rotateEnabled)
			thetadelta += (M_PI/4);
		else if(translateEnabled)
			translatedelta += translatedelta;
        break;
    }
	s = Scale( xscale, yscale, zscale );
	rx = RotateX( xtheta );
	ry = RotateY( ytheta );
	rz = RotateZ( ztheta );
	t = transpose(Translate( xtranslate, ytranslate, ztranslate ));
}

//----------------------------------------------------------------------------

void mainMenu( int id )
{
	switch( id ) {
		case 1:
			scaleEnabled = true;	
			rotateEnabled = false;	
			translateEnabled = false;	
			break;
		case 2:
			scaleEnabled = false;	
			rotateEnabled = true;	
			translateEnabled = false;	
			break;
		case 3:
			scaleEnabled = false;	
			rotateEnabled = false;	
			translateEnabled = true;	
			break;
	}

	glutPostRedisplay();
}
//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	cout << endl;
	cout << "INSTRUCTIONS FOR KEYS" << endl;
	cout << "'q' -  quits the program" << endl;
	cout << endl;
	cout << "'w' -  transformations in the +x direction" << endl;
	cout << "'e' -  transformations in the +y direction" << endl;
	cout << "'r' -  transformations in the +z direction" << endl;
	cout << "'a' -  transformations in the -x direction" << endl;
	cout << "'s' -  transformations in the -y direction" << endl;
	cout << "'d' -  transformations in the -z direction" << endl;
	cout << "'z' -  resets all transformations" << endl;
	cout << endl;
	cout << "'n' -  decreases transformation delta" << endl;
	cout << "'m' -  increases transformation delta" << endl;
	cout << endl;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "COLOR CUBE." );
    glewExperimental=GL_TRUE; 
    glewInit(); 

    init();

	glutCreateMenu( mainMenu ); // Set up menu
	glutAddMenuEntry ( "SCALE", 1 );
	glutAddMenuEntry( "ROTATE", 2 );
	glutAddMenuEntry( "TRANSLATE", 3 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutIdleFunc( idleMain );
    glutDisplayFunc( displayMain );
    glutKeyboardFunc( keyboard );
	glEnable(GL_DEPTH_TEST);

	// Run
    glutMainLoop();
    return 0;
}

