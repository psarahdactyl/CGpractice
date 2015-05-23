// Sarah Kushner
// CS 432
// Assignment 7

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
vec4* points;
vec4* controls;
vec4* vertices;
vec3* normals;
vec4* axes;
int controlsSize;
int verticesSize;
int pointsSize;

//Resolution
int res = 10;
int triIndex = 0;

// Viewing matrices
GLuint model_view;  // model-view matrix uniform shader variable location
GLuint projection; // projection matrix uniform shader variable location

// VAO stuff
GLuint vNormal;
GLuint vPosition;

// Shader options
// phong is 0, flat is 1, neither is 2
GLint shading_type;
int shading = 0;
int former_shading;

// Projection transformation
mat4 p; // projection matrix

// Camera parameters
GLfloat leftA = -1.0, rightA = 1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 0.5, zFar = 3.0;

GLfloat fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat aspect;       // Viewport aspect ratio

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat yVal = 0.5;

float cameraSpeed = 0.5;

// Control Point selection
int CP = 0;

// Control Point size
float CPsize = 4.0;

// Viewing options
bool parallel = true;
bool perspective = false;

// Windows
int mainWin;


struct vec4Compare
{
	bool operator() (const vec4& lhs, const vec4& rhs)
	{
		if(lhs.x == rhs.x && lhs.y == rhs.y)
			return lhs.z < rhs.z;
     	else if (lhs.x == rhs.x)
       	  	return lhs.y < rhs.y;
		else
			return lhs.x < rhs.x;
	}
};

map<vec4, vector<vec3>*, vec4Compare> vertexNormals;

//----------------------------------------------------------------------------

vec3 getNormal(int a, int b, int c)
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[a];

	vec3 normal = normalize( cross(u, v) );
	
	return normal;
}

void addToMap( vec4 key, vec3 value )
{
	vector<vec3> * valueArray;
	try
	{
		valueArray = vertexNormals.at( key );
		valueArray->push_back( value );
	}
	catch(const out_of_range& e) 
	{
		valueArray = new vector<vec3>();
		valueArray->push_back( value );
		vertexNormals.insert( pair<vec4, vector<vec3>*>(key, valueArray) );
	}
}

//--------------------------------------------------------------------------

void tri( int a, int b, int c )
{
	vec3 normal = getNormal(a, b, c);
	points[triIndex] = vertices[a];
	addToMap(vertices[a], normal);
	triIndex++;
	points[triIndex] = vertices[b];
	addToMap(vertices[b], normal);
	triIndex++;
	points[triIndex] = vertices[c];
	addToMap(vertices[c], normal);
	triIndex++;
}

//--------------------------------------------------------------------------

vec3 getVertexNormal( vec4 point )
{
	vector<vec3> * vNorms = vertexNormals.at( point );
	vec3 sum;
	for(unsigned int i = 0; i < vNorms->size(); i++)
	{
		sum += vNorms->at(i);
	}
	return normalize( sum );
}

//--------------------------------------------------------------------------

float b(int i, float u)
{
	float bu;

	if(i == 0)
		bu = pow((1-u), 3);
	else if(i == 1)
		bu = 3 * u * pow( (1 - u), 2 ); 
	else if(i == 2)
		bu = 3 * pow( u, 2 ) * (1 - u);
	else if(i == 3)
		bu = pow( u, 3 );

	return bu;
}

//--------------------------------------------------------------------------

void createPatch()
{
	vertexNormals.clear();
	triIndex = 0;

	verticesSize = res * res;
	int numFaces = 2 * (verticesSize - 2 * res + 1);
	pointsSize = numFaces * 3;

	vertices = new vec4[verticesSize];
	normals = new vec3[pointsSize+controlsSize+6];
	points = new vec4[pointsSize+controlsSize+6];

	int vertIndex = 0;
	float inc = 1/((float)res-1);
	for(float u = 0; u <= 1.0+inc/2; u+=inc)
	{
		for(float v = 0; v <= 1.0+inc/2; v+=inc)
		{
			for(int i = 0; i <= 3; i++)
			{
				float bu = b(i, u);
				for(int j = 0; j <= 3; j++)
				{ 
					// b(u) * b(v) * pij
					int ij = 4*i+j;
					float bv = b(j, v);
					vec4 sumPoint;
					sumPoint = bu * bv * controls[ij];
					vertices[vertIndex] += sumPoint;
				}
			}
			vertIndex++;
		}
	}

	for(int i = 0; i < (vertIndex-res); i++)
	{
		if((i+1) % res == 0)
			continue;

		tri(i, i+res, i+1);
		tri(i+1, i+res, i+res+1);
	}

	for(int i = 0; i < pointsSize; i++)
	{
		normals[i] = getVertexNormal( points[i] );
	}
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
		else
			controlsSize++;
	}

	controls = new vec4[controlsSize];

	myfile.close();
	myfile.open( filename );
	
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

		else
		{
			float x, y, z;
			stringstream datax(tokens.at(0));
			datax >> x;
			stringstream datay(tokens.at(1));
			datay >> y;
			stringstream dataz(tokens.at(2));
			dataz >> z;
			controls[i] = vec4(x/7, y/7, z/7, 1.0);
			i++;
		}
	}

	createPatch();
	int contIndex = 0;
	for(int i = pointsSize; i < controlsSize; i++)
	{
		points[i] = controls[contIndex];
		normals[i] = vec3(1.0, 0.0, 0.0);
		contIndex++;
	}

	axes = new vec4[6];
	axes[0] = vec4(-1.0, 0.0, 0.0);
	axes[1] = vec4(1.0, 0.0, 0.0);
	axes[2] = vec4(0.0, -1.0, 0.0);
	axes[3] = vec4(0.0, 1.0, 0.0);
	axes[4] = vec4(0.0, 0.0, -1.0);
	axes[5] = vec4(.0, 0.0, 1.0);

	int axesIndex = 0;
	for(int i = pointsSize+controlsSize; i < 7; i++)
	{		
		points[i] = axes[axesIndex];
		normals[i] = vec3(1.0, 1.0, 1.0);
		axesIndex++;
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
	vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
					    BUFFER_OFFSET(0) );

	vNormal = glGetAttribLocation( program, "vNormal" );
	glEnableVertexAttribArray( vNormal );
	glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));

	// Initialize shader lighting parameters
	vec4 light_position1( 0.4, 0.1, 0.4, 1.0 );
	vec4 light_position2( 0.7, 0.4, 0.2, 1.0 );

	vec4 light_diffuse1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_diffuse2( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_ambient1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_ambient2( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_specular1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_specular2( 0.5, 0.5, 0.5, 1.0 );

	vec4 material_diffuse = vec4( 1, 0.829, 0.829, 1.0 );
	vec4 material_ambient = vec4( 0.25, 0.20725, 0.20725, 1.0 );
	vec4 material_specular = vec4( 0.296648, 0.296648, 0.296648, 1.0 );
	float material_shininess = 0.08;

	vec4 diffuse_product1 = light_diffuse1 * material_diffuse;
	vec4 diffuse_product2 = light_diffuse2 * material_diffuse;
	vec4 ambient_product1 = light_ambient1 * material_ambient;
	vec4 ambient_product2 = light_ambient2 * material_ambient;
	vec4 specular_product1 = light_specular1 * material_specular;
	vec4 specular_product2 = light_specular2 * material_specular;


	// Uniforms
	glUniform4fv( glGetUniformLocation(program, "DiffuseProduct1"),
					1, diffuse_product1 );
	glUniform4fv( glGetUniformLocation(program, "DiffuseProduct2"),
					1, diffuse_product2 );

	glUniform4fv( glGetUniformLocation(program, "AmbientProduct1"),
					1, ambient_product1 );
	glUniform4fv( glGetUniformLocation(program, "AmbientProduct2"),
					1, ambient_product2 );

	glUniform4fv( glGetUniformLocation(program, "SpecularProduct1"),
					1, specular_product1 );
	glUniform4fv( glGetUniformLocation(program, "SpecularProduct2"),
					1, specular_product2 );

	glUniform1f( glGetUniformLocation(program, "Shininess"),
					material_shininess );

	glUniform4fv( glGetUniformLocation(program, "LightPosition1"),
					1, light_position1 );
	glUniform4fv( glGetUniformLocation(program, "LightPosition2"),
					1, light_position2 );

    model_view = glGetUniformLocation( program, "model_view" );
	projection = glGetUniformLocation( program, "projection" );

	shading_type = glGetUniformLocation( program, "shading_type" );

	glClearColor( 1.0, 1.0, 1.0, 1.0 );

}

//----------------------------------------------------------------------------

void displayMain( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

    vec4  eye( radius*sin(theta)+0.4, yVal+0.1, radius*cos(theta)+0.3, 1.0 );
	vec4  at( 0.4, 0.1, 0.3, 1.0 );
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

	glUniform1i( shading_type, shading );

	glDrawArrays( GL_TRIANGLES, 0, pointsSize);
	glPointSize( CPsize );
	//shading = 2;
	glDrawArrays( GL_POINTS, pointsSize, controlsSize );
	glDrawArrays( GL_LINES, pointsSize+controlsSize, 2 );
	glDrawArrays( GL_LINES, pointsSize+controlsSize+2, 2 );
	glDrawArrays( GL_LINES, pointsSize+controlsSize+4, 2 );

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idleMain( void )
{
	GLfloat dr = cameraSpeed * DegreesToRadians;
	theta += dr; 
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
  	case 'x':
		controls[CP-1].x += 0.22;
		createPatch();
		loadBuffer();
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));
		break;
  	case 'X': 
		controls[CP-1].x -= 0.22;	
		createPatch();
		loadBuffer();
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));
		break;
 	case 'y': 
		controls[CP-1].y += 0.22;	
		createPatch();
		loadBuffer();
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));
		break;
 	case 'Y': 
		controls[CP-1].y -= 0.22;	
		createPatch();
		loadBuffer();
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));
		break;
  	case 'z': 
		controls[CP-1].z += 0.22;	
		createPatch();
		loadBuffer();
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));
		break;
  	case 'Z': 
		controls[CP-1].z -= 0.22;	
		createPatch();
		loadBuffer();
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));
		break;
 	case '1': //decrease resolution
		if(res > 10)
			res -= 10;
		createPatch();
		loadBuffer();
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));
		break;
 	case '2': //increase resolution
		res += 10;
		createPatch();
		loadBuffer();
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize));
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

void projMenu( int id )
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
	}

	glutPostRedisplay();
}


void animMenu( int id )
{
	switch( id ) {
		case 1:
			glutIdleFunc( NULL );
			break;
		case 2:
			glutIdleFunc( idleMain );
			break;
	}

	glutPostRedisplay();
}

void shadMenu( int id )
{
	switch( id ) {
		case 1:
			shading = 0;
			break;
		case 2:
			shading = 1;	
			break;
	}

	glutPostRedisplay();
}

void CPMenu( int id )
{
	switch( id ) {
		case 1:
			CP = 1;
			break;
		case 2:
			CP = 2;
			break;
		case 3:
			CP = 3;
			break;
		case 4:
			CP = 4;
			break;
		case 5:
			CP = 5;
			break;
		case 6:
			CP = 6;
			break;
		case 7:
			CP = 7;
			break;
		case 8:
			CP = 8;
			break;
		case 9:
			CP = 9;
			break;
		case 10:
			CP = 10;
			break;
		case 11:
			CP = 11;
			break;
		case 12:
			CP = 12;
			break;
		case 13:
			CP = 13;
			break;
		case 14:
			CP = 14;
			break;
		case 15:
			CP = 15;
			break;
		case 16:
			CP = 16;
			break;
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{

	makeVertices( argv[1] );

	cout << endl;
	cout << "---------------------" << endl;
	cout << "INSTRUCTIONS FOR KEYS" << endl;
	cout << "---------------------" << endl;
	cout << "Right click to see a nice drop down menu!" << endl;
	cout << endl;
	cout << "Use the drop down to select a control point then press the keys below:" << endl;
	cout << "'x (lowercase)' - increases the control point in the x-direction " << endl;
	cout << "'X (uppercase)' - decreases the control point in the x-direction " << endl;
	cout << "'y (lowercase)' - increases the control point in the y-direction " << endl;
	cout << "'Y (uppercase)' - decreases the control point in the y-direction " << endl;
	cout << "'z (lowercase)' - increases the control point in the z-direction " << endl;
	cout << "'Z (uppercase)' - decreases the control point in the z-direction " << endl;
	cout << endl;
	cout << "'s (lowercase)' - increases size of all control points " << endl;
	cout << "'S (uppercase)' - decreases size of all control points " << endl;
	cout << endl;
	cout << "'1' - decreases resolution " << endl;
	cout << "'2' - increases resolution " << endl;
	cout << endl;
	cout << "The drop down menu can also be used for starting/stopping the animation." << endl;
	cout << "Shading can be changed from flat to smooth also." << endl;
	cout << endl;
	cout << "'q' -  quits the program" << endl;
	cout << endl;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "Dude, it's a Bezier Patch!" );
    glewExperimental=GL_TRUE; 
    glewInit(); 

    init();

	int projectionMenu = glutCreateMenu( projMenu );
	glutAddMenuEntry( "Parallel", 1 );
	glutAddMenuEntry( "Perspective", 2 );

	int shadingMenu = glutCreateMenu( shadMenu );
	glutAddMenuEntry( "Phong", 1 );
	glutAddMenuEntry( "Flat", 2 );

	int animationMenu = glutCreateMenu( animMenu );
	glutAddMenuEntry( "Stop Animation", 1 );
	glutAddMenuEntry( "Start Animation", 2 );

	int controlMenu = glutCreateMenu( CPMenu );
	glutAddMenuEntry( "1", 1 );
	glutAddMenuEntry( "2", 2 );
	glutAddMenuEntry( "3", 3 );
	glutAddMenuEntry( "4", 4 );
	glutAddMenuEntry( "5", 5 );
	glutAddMenuEntry( "6", 6 );
	glutAddMenuEntry( "7", 7 );
	glutAddMenuEntry( "8", 8 );
	glutAddMenuEntry( "9", 9 );
	glutAddMenuEntry( "10", 10 );
	glutAddMenuEntry( "11", 11 );
	glutAddMenuEntry( "12", 12 );
	glutAddMenuEntry( "13", 13 );
	glutAddMenuEntry( "14", 14 );
	glutAddMenuEntry( "15", 15 );
	glutAddMenuEntry( "16", 16 );

	glutCreateMenu( mainMenu ); // Set up menu
	glutAddSubMenu( "Projection", projectionMenu );
	glutAddSubMenu( "Shading", shadingMenu );
	glutAddSubMenu( "Animation", animationMenu );
	glutAddSubMenu( "Controls", controlMenu );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutIdleFunc( idleMain );
    glutDisplayFunc( displayMain );
    glutReshapeFunc( reshapeMain );
    glutKeyboardFunc( keyboard );
	glEnable(GL_DEPTH_TEST);

	// Run
    glutMainLoop();
    return 0;
}

