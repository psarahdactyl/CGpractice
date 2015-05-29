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
#include <map>
#include <stdexcept>

using namespace std;

// Globals, i'm so sorry
GLuint program;

// Point and Color arrays
vec4* points;
vec4* colors; 
vec4* vertices;
vec3* normals;
int verticesSize;
int pointsSize;

// Viewing matrices
GLuint model_view;  // model-view matrix uniform shader variable location
GLuint projection; // projection matrix uniform shader variable location

// Shader options
// gouraud is 0, phong is 1
GLint shading_type;
int shading = 0;

// Material Options
vec4 material_diffuse;
vec4 material_ambient;
vec4 material_specular;
float material_shininess;

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

GLfloat radiusLight = 0.7;
GLfloat thetaLight = 0.0;
GLfloat yValLight = 0.5;

float cameraSpeed = 1.0;

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

//--------------------------------------------------------------------------

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

int Index = 0;
void tri( int a, int b, int c )
{
	vec3 normal = getNormal(a, b, c);
	points[Index] = vertices[a];
	addToMap(vertices[a], normal);
	Index++;
	points[Index] = vertices[b];
	addToMap(vertices[b], normal);
	Index++;
	points[Index] = vertices[c];
	addToMap(vertices[c], normal);
	Index++;
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
	cout << "does this take " << endl;
	for(int i = 0; i < pointsSize; i++)
	{
		normals[i] = getVertexNormal( points[i] );
	}
	cout << "forver? " << endl;
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

void changeMaterial( GLuint program )
{
	// Initialize shader lighting parameters
	vec4 light_position1( radius*sin(theta), yVal, radius*cos(theta), 1.0 );
	vec4 light_position2( radiusLight*sin(thetaLight), yValLight, radiusLight*cos(thetaLight), 1.0 );

	vec4 light_diffuse1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_diffuse2( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_ambient1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_ambient2( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_specular1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_specular2( 0.5, 0.5, 0.5, 1.0 );

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
	program = InitShader( "vshdr.glsl", "fshdr.glsl" );
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


	material_diffuse = vec4( 0.07568, 0.61424, 0.07568, 1.0 );
	material_ambient = vec4( 0.0215, 0.1745, 0.0215, 1.0 );
	material_specular = vec4( 0.628281, 0.555802, 0.366065, 1.0 );
	material_shininess = 0.4;

	changeMaterial( program );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );

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

	glUniform1i( shading_type, shading );

	glDrawArrays( GL_TRIANGLES, 0, pointsSize);

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
	GLfloat dr = 1.0 * DegreesToRadians;
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
  	case 'l': 
		radiusLight *= 1.5; 
		changeMaterial( program );
		break;
  	case 'L': 
		radiusLight *= 0.667; 
		changeMaterial( program );
		break;
 	case 'p': 
		yValLight += dr; 
		changeMaterial( program );
		break;
 	case 'P': 
		yValLight -= dr; 
		changeMaterial( program );
		break;
 	case 't': 
		thetaLight += 3*dr; 
		changeMaterial( program );
		break;
 	case 'T': 
		thetaLight -= 3*dr; 
		changeMaterial( program );
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
		radiusLight = 0.7;
		thetaLight = 0.0;
		yValLight = 0.5;
		changeMaterial( program );
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

void matMenu( int id )
{
	switch( id ) {
		case 1:
			// Gold
			material_diffuse = vec4( 0.75164, 0.60648, 0.22648, 1.0 );
			material_ambient = vec4( 0.24725, 0.1995, 0.0745, 1.0 );
			material_specular = vec4( 0.633, 0.727811, 0.633, 1.0 );
			material_shininess = 0.6;
			changeMaterial( program );
			break;
		case 2:
			// Emerald
			material_diffuse = vec4( 0.07568, 0.61424, 0.07568, 1.0 );
			material_ambient = vec4( 0.0215, 0.1745, 0.0215, 1.0 );
			material_specular = vec4( 0.628281, 0.555802, 0.366065, 1.0 );
			material_shininess = 0.4;
			changeMaterial( program );
			break;
		case 3:
			// Metal
			material_diffuse = vec4( 0.780392, 0.568627, 0.113725, 1.0 );
			material_ambient = vec4( 0.329412, 0.223529, 0.027451, 1.0 );
			material_specular = vec4( 0.992157, 0.941176, 0.807843, 1.0 );
			material_shininess = 0.21794872;
			changeMaterial( program );
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
	cout << endl;
	cout << "'t' -  rotates light around model on cylinder counter-clockwise" << endl;
	cout << "'T' -  rotates light around model on cylinder clockwise" << endl;
	cout << "'l' -  increases orbit radius of light" << endl;
	cout << "'L' -  decreases orbit radius of light" << endl;
	cout << "'p' -  increases height of light on cylinder" << endl;
	cout << "'P' -  decreases height of light on cylinder" << endl;
	cout << endl;
	cout << "'z' -  resets all transformations" << endl;
	cout << endl;
	cout << "DROP DOWN MENUS" << endl;
	cout << "-Projections" << endl;
	cout << "  -Parallel" << endl;
	cout << "  -Perspective" << endl;
	cout << "-Materials" << endl;
	cout << "  -Gold" << endl;
	cout << "  -Emerald" << endl;
	cout << "  -Brass" << endl;
	cout << "-Shading" << endl;
	cout << "  -Gouraud" << endl;
	cout << "  -Phong" << endl;
	cout << "-Animation" << endl;
	cout << "  -Start Animation" << endl;
	cout << "  -Stop Animation" << endl;
	cout << endl;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "MODELS AND SHADING, YO." );
    glewExperimental=GL_TRUE; 
    glewInit(); 

    init();

	int projectionMenu = glutCreateMenu( projMenu );
	glutAddMenuEntry( "Parallel", 1 );
	glutAddMenuEntry( "Perspective", 2 );

	int materialsMenu = glutCreateMenu( matMenu );
	glutAddMenuEntry( "Gold", 1 );
	glutAddMenuEntry( "Emerald", 2 );
	glutAddMenuEntry( "Brass", 3 );

	int shadingMenu = glutCreateMenu( shadMenu );
	glutAddMenuEntry( "Gouraud", 1 );
	glutAddMenuEntry( "Phong", 2 );

	int animationMenu = glutCreateMenu( animMenu );
	glutAddMenuEntry( "Stop Animation", 1 );
	glutAddMenuEntry( "Start Animation", 2 );

	glutCreateMenu( mainMenu ); // Set up menu
	glutAddSubMenu( "Projection", projectionMenu );
	glutAddSubMenu( "Materials", materialsMenu );
	glutAddSubMenu( "Shading", shadingMenu );
	glutAddSubMenu( "Animation", animationMenu );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutIdleFunc( idleMain );
    glutDisplayFunc( displayMain );
    glutReshapeFunc( reshapeMain );
    glutKeyboardFunc( keyboard );
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Run
    glutMainLoop();
    return 0;
}

