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

// Globals, i'm so sorry

// Point and Color arrays
vec4* points;
vec4* colors; 
vec4* controls;
vec4* vertices;
vec3* normals;
int controlsSize;
int verticesSize;
int pointsSize;

//Resolution
int res = 10;

// Viewing matrices
GLuint model_view;  // model-view matrix uniform shader variable location
GLuint projection; // projection matrix uniform shader variable location

// Shader options
// phong is 0, flat is 1
GLint shading_type;
int shading = 0;


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

int triIndex = 0;
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
		//else if(tokens.at(0) == "v")
		//	verticesSize++;
		//else if(tokens.at(0) == "f")
		//	pointsSize++;
		else
			controlsSize++;
	}

	verticesSize = res * res;
	int numFaces = 2 * (verticesSize - 2 * res + 1);
	pointsSize = numFaces * 3;

	vertices = new vec4[verticesSize];
	normals = new vec3[pointsSize];
	controls = new vec4[controlsSize];
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

	int vertIndex = 0;
	float inc = 1/((float)res-1);
	for(float u = 0; u <= 1.0; u+=inc)
	{
		for(float v = 0; v <= 1.0; v+=inc)
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
					sumPoint.x = bu * bv * controls[ij].x;
					sumPoint.y = bu * bv * controls[ij].y;
					sumPoint.z = bu * bv * controls[ij].z;
					vertices[vertIndex] += sumPoint;
				}
			}
			//cout << "# " << vertIndex << ":    " << vertices[vertIndex].x << " " << vertices[vertIndex].y << " " << vertices[vertIndex].z << endl;
			vertIndex++;
		}
	}
	for(int i = 0; i < (vertIndex-res); i++)
	{
		if((i+1) % 10 == 0)
			continue;

		tri(i, i+10, i+1);
		tri(i+1, i+10, i+11);
		//cout << i << " " << i+10 << " " << i+1 << endl;
		//cout << i+1 << " " << i+10 << " " << i+11 << endl;
	}
	for(int i = 0; i < pointsSize; i++)
	{
		//cout << "# " << i << ":    " << points[i].x << " " << points[i].y << " " << points[i].z << endl;
		normals[i] = getVertexNormal( points[i] );
	}
}

//--------------------------------------------------------------------------

void loadBuffer( void )
{
	// Load array into buffer
	//glBufferData( GL_ARRAY_BUFFER, sizeof(*points)*pointsSize +
	//						 sizeof(*normals)*pointsSize, NULL, GL_STATIC_DRAW );
	//glBufferSubData( GL_ARRAY_BUFFER, 0,
	//						  sizeof(*points)*pointsSize, points );
	//glBufferSubData( GL_ARRAY_BUFFER, sizeof(*points)*pointsSize,
	//						   sizeof(*normals)*pointsSize, normals );

	glBufferData( GL_ARRAY_BUFFER, sizeof(*controls)*controlsSize +
							 sizeof(*normals)*pointsSize, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,
							  sizeof(*controls)*controlsSize, controls );
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
	vec4 light_position1( 1.0, 1.0, 0.0, 1.0 );
	vec4 light_position2( 1.0, 0.0, 1.0, 1.0 );

	vec4 light_diffuse1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_diffuse2( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_ambient1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_ambient2( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_specular1( 0.5, 0.5, 0.5, 1.0 );
	vec4 light_specular2( 0.5, 0.5, 0.5, 1.0 );

	vec4 material_diffuse = vec4( 0.07568, 0.61424, 0.07568, 1.0 );
	vec4 material_ambient = vec4( 0.0215, 0.1745, 0.0215, 1.0 );
	vec4 material_specular = vec4( 0.628281, 0.555802, 0.366065, 1.0 );
	float material_shininess = 0.4;

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

	glClearColor( 0.5, 0.5, 0.5, 1.0 );

}

//----------------------------------------------------------------------------

void displayMain( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

	glPointSize( 10.0 );
    vec4  eye( radius*sin(theta), yVal, radius*cos(theta), 1.0 );
	vec4  at( 0.0, 0.0,  0.0, 1.0 );
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

	glDrawArrays( GL_POINTS, 0, pointsSize);

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
  	case 'r': 
		break;
  	case 'R': 
		break;
 	case 'h': 
		break;
 	case 'H': 
		break;
  	case 'l': 
		break;
  	case 'L': 
		break;
 	case 'p': 
		break;
 	case 'P': 
		break;
 	case 't': 
		break;
 	case 'T': 
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
	cout << endl;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "Dude, it's a Bezier Patch!" );
    glewExperimental=GL_TRUE; 
    glewInit(); 

    init();

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

