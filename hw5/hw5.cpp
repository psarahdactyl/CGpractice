// Sarah Kushner
// CS 432
// Assignment 4

#include "Angel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

// Point and Color arrays
vec4* points;
vec4* colors; 
vec4* vertices;
vec3* normals;
int verticesSize;
int pointsSize;

GLuint model_view;  // model-view matrix uniform shader variable location
GLuint projection; // projection matrix uniform shader variable location

// Projection transformation parameters
mat4 p; // projection matrix

GLfloat leftA = -1.0, rightA = 1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 0.5, zFar = 3.0;

GLfloat fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat aspect;       // Viewport aspect ratio

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat yVal = 0.5;
//GLfloat phi = 0.0;

float cameraSpeed = 3.0;

//Viewing options
bool parallel = true;
bool perspective = false;

// Windows
int mainWin;

//--------------------------------------------------------------------------

vec3 getNormal(int a, int b, int c)
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[a];

	vec3 normal = normalize( cross(u, v) );
	
	return normal;
}

int Index = 0;
void tri( int a, int b, int c )
{
	vec3 normal = getNormal(a, b, c);
	//colors[Index] = vertex_colors[a % 8]; 
	points[Index] = vertices[a];
	normals[Index] = normal;
	Index++;
	//colors[Index] = vertex_colors[b % 8]; 
	points[Index] = vertices[b]; 
	normals[Index] = normal;
	Index++;
	//colors[Index] = vertex_colors[c % 8]; 
	points[Index] = vertices[c]; 
	normals[Index] = normal;
	Index++;

}

//--------------------------------------------------------------------------

void makeVertices( char * filename )
{
	// open file
	ifstream myfile;
	myfile.open( filename );

	string line;

	// read file
	pointsSize = 0;
	verticesSize = 0;
	while (getline( myfile, line ))
	{
		istringstream iss( line );
		vector<string> tokens;
		copy(istream_iterator<string>(iss),
			 istream_iterator<string>(),
			 back_inserter(tokens));

		if(tokens.size() == 0)
			continue;
		else if(tokens.at(0) == "v")
			verticesSize++;
		else if(tokens.at(0) == "f")
			pointsSize++;
	}

	pointsSize *= 3;

	vertices = new vec4[verticesSize];
	normals = new vec3[pointsSize];
	points = new vec4[pointsSize];
	colors = new vec4[pointsSize];

	myfile.close();
	myfile.open( filename );
	
	//cout << pointsSize << endl;

	int i = 0;
	while (getline( myfile, line ))
	{
		istringstream iss( line );
		vector<string> tokens;
		copy(istream_iterator<string>(iss),
			 istream_iterator<string>(),
			 back_inserter(tokens));

		if(tokens.size() == 0)
			continue;

		else if(tokens.at(0) == "v")
		{
			float x, y, z;
			stringstream datax(tokens.at(1));
			datax >> x;
			stringstream datay(tokens.at(2));
			datay >> y;
			stringstream dataz(tokens.at(3));
			dataz >> z;
			vertices[i] = vec4(x, y, z, 1.0);
			i++;
		}
		else if(tokens.at(0) == "f")
		{
			int v1, v2, v3;	
			stringstream datav1(tokens.at(1));
			datav1 >> v1;
			stringstream datav2(tokens.at(2));
			datav2 >> v2;
			stringstream datav3(tokens.at(3));
			datav3 >> v3;

			tri(v1-1, v2-1, v3-1);
		}
	}
}

//--------------------------------------------------------------------------

void loadBuffer( void )
{
	// Load array into buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(*points)*pointsSize +
							 sizeof(*normals)*pointsSize, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,
							  sizeof(*points)*pointsSize, points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(*points)*pointsSize,
							   sizeof(*normals)*pointsSize, normals );


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

	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "vshdr.glsl", "fshdr.glsl" );
	glUseProgram( program );

	// Set up vertex arrays
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
					    BUFFER_OFFSET(0) );

	GLuint vNormal = glGetAttribLocation( program, "vNormal" );
	glEnableVertexAttribArray( vNormal );
	glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize) );

	// Initialize shader lighting parameters
	vec4 light_position1( 0.0, 0.0, -1.0, 0.0 );
	vec4 light_position2( 0.0, 1.0, -1.0, 0.0 );
	vec4 light_diffuse1( 1.0, 1.0, 0.0, 1.0 );
	vec4 light_diffuse2( 1.0, 0.0, 1.0, 1.0 );

	vec4 material_diffuse1( 1.0, 0.1, 0.0, 1.0 );
	vec4 material_diffuse2( 1.0, 0.2, 0.0, 1.0 );

	vec4 diffuse_product1 = light_diffuse1 * material_diffuse1;
	vec4 diffuse_product2 = light_diffuse2 * material_diffuse2;

	// Uniforms
	glUniform4fv( glGetUniformLocation(program, "DiffuseProduct1"),
					1, diffuse_product1 );
	glUniform4fv( glGetUniformLocation(program, "DiffuseProduct2"),
					1, diffuse_product2 );

	glUniform4fv( glGetUniformLocation(program, "LightPosition1"),
					1, light_position1 );
	glUniform4fv( glGetUniformLocation(program, "LightPosition2"),
					1, light_position2 );

    model_view = glGetUniformLocation( program, "model_view" );
	projection = glGetUniformLocation( program, "projection" );


	glClearColor( 1.0, 1.0, 1.0, 1.0 );

}

//----------------------------------------------------------------------------

void displayMain( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

    vec4  eye( radius*sin(theta), yVal, radius*cos(theta), 1.0 );
	vec4  at( 0.0, 0.0, 0.0, 1.0 );
	vec4 up( 0.0, 1.0, 0.0, 0.0 );

	mat4 mv = LookAt( eye, at, up );
	glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );

	// Orthographic
	if( parallel )
		p = Ortho( leftA, rightA, bottom, top, zNear, zFar );
	// Perspective
	else if( perspective )
		p = Perspective( fovy, aspect, zNear, zFar );

	glUniformMatrix4fv( projection, 1, GL_TRUE, p );

	glDrawArrays( GL_TRIANGLES, 0, pointsSize);

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idleMain( void )
{
	GLfloat dr = cameraSpeed * DegreesToRadians;
	theta += dr; 
	//cout << theta << endl;
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
	GLfloat dr = 3.0 * DegreesToRadians;
    switch ( key ) {
    case 'q':
        exit( EXIT_SUCCESS );
        break;
  	case '+': 
		cameraSpeed += 0.5; 
		break;
  	case '-': 
		cameraSpeed -= 0.5; 
		break;
  	case 'r': 
		radius *= 1.5; 
		break;
  	case 'R': 
		radius *= 0.667; 
		break;
 	case 'h': 
		yVal += dr; 
		break;
 	case 'H': 
		yVal -= dr; 
		break;
	case 'z':  // reset values to their defaults
		leftA = -1.0;
		rightA = 1.0;
		bottom = -1.0;
		top = 1.0;
		zNear = 0.5;
		zFar = 3.0;
		radius = 1.0;
		theta = 0.0;
		yVal = 1.0;
		break;
	}
}

//----------------------------------------------------------------------------

void mainMenu( int id )
{
	switch( id ) {
		case 1:
			perspective = false;	
			parallel = true;	
			break;
		case 2:
			parallel = false;	
			perspective = true;	
			break;
		case 3:
			glutIdleFunc( NULL );
			break;
		case 4:
			glutIdleFunc( idleMain );
			break;
	}

	glutPostRedisplay();
}


//----------------------------------------------------------------------------

int main( int argc, char **argv )
{

	makeVertices( argv[1] );

	cout << endl;
	cout << "INSTRUCTIONS FOR KEYS" << endl;
	cout << "'q' -  quits the program" << endl;
	cout << endl;
	cout << "'+' -  increases speed of rotation" << endl;
	cout << "'-' -  decreases speed of rotation" << endl;
	cout << "'r' -  increases orbit radius" << endl;
	cout << "'R' -  decreases orbit radius" << endl;
	cout << "'h' -  increases height of camera on cylinder" << endl;
	cout << "'H' -  decreases height of camera on cylinder" << endl;
	cout << "'z' -  resets all transformations" << endl;
	cout << endl;
	cout << "There is also a drop down menu to start and stop the rotation" << endl;
	cout << "and to switch between parallel and perspective projections." << endl;
	cout << endl;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "MODELS, YO." );
    glewExperimental=GL_TRUE; 
    glewInit(); 

    init();

	glutCreateMenu( mainMenu ); // Set up menu
	glutAddMenuEntry ( "Parallel Projection", 1 );
	glutAddMenuEntry( "Perspective Projection", 2 );
	glutAddMenuEntry( "Stop Animation", 3 );
	glutAddMenuEntry( "Start Animation", 4 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutIdleFunc( idleMain );
    glutDisplayFunc( displayMain );
    glutReshapeFunc( reshapeMain );
    glutKeyboardFunc( keyboard );
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	// Run
    glutMainLoop();
    return 0;
}

