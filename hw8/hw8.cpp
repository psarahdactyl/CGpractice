// Sarah Kushner
// CS 432
// Assignment 8

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
GLuint vao, buffer, fb, color_rb, depth_rb;
GLuint vNormal, vPosition, vColor, program;

// Point and Color arrays
vec4* points;
vec4* colors; 
vec4* vertices;
vec3* normals;

int verticesSize;
int pointsSize;
int firstPoints = 0;
int secondPoints = 0;
int thirdPoints = 0;

// Viewing matrices
GLuint model_view;  // model-view matrix uniform shader variable location
GLuint projection; // projection matrix uniform shader variable location

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

bool MSAA = false;
bool click0 = false;
bool click1 = false;
bool click2 = false;

vec4 diffuse0 = vec4( 0.07568, 0.61424, 0.07568, 1.0 );
vec4 diffuse1 = vec4( 0.61424, 0.04136, 0.04136, 1.0 );
vec4 diffuse2 = vec4( 0.07568, 0.61424, 0.07568, 1.0 );

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

//----------------------------------------------------------------------------

void calculatePoints(char * filename, float xoffset, float yoffset, float zoffset, float scale)
{
	ifstream myfile;
	myfile.open( filename );
	
	int i = 0;
	string line;
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
			vertices[i] = vec4(x/scale-xoffset, y/scale-yoffset, z/scale-zoffset, 1.0);
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

void makeVertices( )
{
	char * files[3] = { "bound-bunny_5k.smf", "penguin.smf", "frog.smf" };

	pointsSize = 0;
	for(int i = 0; i < 3; i++)
	{
		// open file
		ifstream myfile;
		myfile.open( files[i] );

		string line;

		// read file
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
			{
				pointsSize++;
				if(i == 0)
					firstPoints++;
				else if(i == 1)
					secondPoints++;
				else if(i == 2)
					thirdPoints++;
			}
		}
		myfile.close();
	}
	firstPoints *= 3;
	secondPoints *= 3;
	thirdPoints *= 3;
	pointsSize *= 3;

	vertices = new vec4[verticesSize];
	normals = new vec3[pointsSize];
	points = new vec4[pointsSize];
	colors = new vec4[pointsSize];

	calculatePoints(files[0], 0.2, -0.2, 0, 3);
	calculatePoints(files[1], 0.1, -0.6, 0.7, 2);
	calculatePoints(files[2], -0.3, -0.2, -0.4, 6);

	for(int i = 0; i < pointsSize; i++)
	{
		if(i < firstPoints)
			colors[i] = vec4( 1.0, 0.0, 0.0, 1.0 );
		else if(i < firstPoints+secondPoints)
			colors[i] = vec4( 0.0, 1.0, 0.0, 1.0 );
		else if(i < pointsSize)
			colors[i] = vec4( 0.0, 0.0, 1.0, 1.0 );
	}

	for(int i = 0; i < pointsSize; i++)
	{
		normals[i] = getVertexNormal( points[i] );
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

void changeMaterial( GLuint program )
{
	// Initialize shader lighting parameters
	vec4 light_position1( 1.0, 0.5, 0.0, 1.0 );

	vec4 light_diffuse1( 0.8, 0.8, 0.8, 1.0 );
	vec4 light_ambient1( 0.7, 0.7, 0.7, 1.0 );
	vec4 light_specular1( 0.5, 0.5, 0.5, 1.0 );

	vec4 diffuse_product1 = light_diffuse1 * material_diffuse;
	vec4 ambient_product1 = light_ambient1 * material_ambient;
	vec4 specular_product1 = light_specular1 * material_specular;

	// Uniforms
	glUniform4fv( glGetUniformLocation(program, "DiffuseProduct1"),
					1, diffuse_product1 );

	glUniform4fv( glGetUniformLocation(program, "AmbientProduct1"),
					1, ambient_product1 );

	glUniform4fv( glGetUniformLocation(program, "SpecularProduct1"),
					1, specular_product1 );

	glUniform1f( glGetUniformLocation(program, "Shininess"),
					material_shininess );

	glUniform4fv( glGetUniformLocation(program, "LightPosition1"),
					1, light_position1 );

    model_view = glGetUniformLocation( program, "model_view" );
	projection = glGetUniformLocation( program, "projection" );


}


//----------------------------------------------------------------------------

void useSmoothShading( void )
{
	// Load shaders and use the resulting shader program
	program = InitShader( "vshdr.glsl", "fshdr.glsl" );
	changeMaterial( program );

	// Set up camera 
    vec4 eye( radius*sin(theta), yVal, radius*cos(theta), 1.0 );
	vec4 at( 0.0, 0.0, 0.0, 1.0 );
	vec4 up( 0.0, 1.0, 0.0, 0.0 );

	mat4 mv = LookAt( eye, at, up );
	glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );

	// Perspective projection
	p = Perspective( fovy, aspect, zNear, zFar );
	glUniformMatrix4fv( projection, 1, GL_TRUE, p );

	vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
					    BUFFER_OFFSET(0) );

	vNormal = glGetAttribLocation( program, "vNormal" );
	glEnableVertexAttribArray( vNormal );
	glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize) );
	loadBuffer();

	glUseProgram( program );
	
}

//----------------------------------------------------------------------------

void useColors( void )
{
	program = InitShader( "vshdrColor.glsl", "fshdr.glsl" );
	changeMaterial( program );

	// Set up camera 
    vec4 eye( radius*sin(theta), yVal, radius*cos(theta), 1.0 );
	vec4 at( 0.0, 0.0, 0.0, 1.0 );
	vec4 up( 0.0, 1.0, 0.0, 0.0 );

	mat4 mv = LookAt( eye, at, up );
	glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );

	// Perspective projection
	p = Perspective( fovy, aspect, zNear, zFar );
	glUniformMatrix4fv( projection, 1, GL_TRUE, p );

	vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
					    BUFFER_OFFSET(0) );

	vColor = glGetAttribLocation( program, "vColor" );
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET(sizeof(*points)*pointsSize) );

	glBufferData( GL_ARRAY_BUFFER, sizeof(*points)*pointsSize +
							 sizeof(*colors)*pointsSize, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,
							  sizeof(*points)*pointsSize, points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(*points)*pointsSize,
							   sizeof(*colors)*pointsSize, colors );

	glUseProgram( program );
}

//--------------------------------------------------------------------------

void init( )
{
		
	//RGBA8 RenderBuffer, 24 bit depth RenderBuffer
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);

	//Create and attach a color buffer
	glGenRenderbuffers(1, &color_rb);
	glBindRenderbuffer(GL_RENDERBUFFER, color_rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 500, 500);

	//Attach color buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_rb);

	//Create and attach a depth buffer
	glGenRenderbuffers(1, &depth_rb);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 500, 500);

	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
	
	// Use correct shader
	useSmoothShading();

	glClearColor( 0.0, 0.0, 0.0, 1.0 );

}


//----------------------------------------------------------------------------

void displayMain( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//---------------
	// Render start 
	//---------------

	// Render to the screen
	// Object 1 (bunny)
	material_diffuse = diffuse0;
	material_ambient = vec4( 0.0215, 0.1745, 0.0215, 1.0 );
	material_specular = vec4( 0.628281, 0.555802, 0.366065, 1.0 );
	material_shininess = 0.4;
	changeMaterial( program );
	glDrawArrays( GL_TRIANGLES, 0, firstPoints-1);

	// Object 2 (penguin)
	material_diffuse = diffuse1;
	material_ambient = vec4( 0.1745, 0.01175, 0.01175, 1.0 );
	material_specular = vec4( 0.727811, 0.626959, 0.626959, 1.0 );
	material_shininess = 0.6;
	changeMaterial( program );
	glDrawArrays( GL_TRIANGLES, firstPoints, secondPoints-1);

	// Object 3 (frog)
	material_diffuse = diffuse2;
	material_ambient = vec4( 0.329412, 0.223529, 0.027451, 1.0 );
	material_specular = vec4( 0.992157, 0.941176, 0.807843, 1.0 );
	material_shininess = 0.21794872;
	changeMaterial( program );
	glDrawArrays( GL_TRIANGLES, secondPoints, thirdPoints-1);

	glFlush();
	glutSwapBuffers();

	//----------------------------------------
	// Render separation for reading purposes
	//----------------------------------------


	// Render to the FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport( 0, 0, 500, 500 );
	
	useColors();
	glBindBuffer( GL_ARRAY_BUFFER, buffer );	

	// Render into frame buffer
	glDrawArrays( GL_TRIANGLES, 0, firstPoints-1);

	glDrawArrays( GL_TRIANGLES, firstPoints, secondPoints-1);

	glDrawArrays( GL_TRIANGLES, secondPoints, thirdPoints-1);

	glFlush();
	glutSwapBuffers();


	// Bind 0 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	useSmoothShading();
	glBindBuffer( GL_ARRAY_BUFFER, buffer );	


	
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
	GLfloat dr = 1.0 * DegreesToRadians;
    switch ( key ) {
    case 'q':
        exit( EXIT_SUCCESS );
        break;
  	case '=': 
		theta += dr; 
		break;
  	case '-': 
		theta -= dr;
		break;
  	case 'r': 
		radius += 0.2; 
		break;
  	case 'R': 
		radius -= 0.2; 
		break;
 	case 'h': 
		yVal += dr;	
		break;
 	case 'H': 
		yVal -= dr; 
		break;
 	case 'a':
		MSAA = !MSAA;
		if(MSAA)
			glEnable( GL_MULTISAMPLE_ARB );
		else
			glDisable( GL_MULTISAMPLE_ARB );
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
		yVal = 0.5;
		break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void mouseMain( int button, int state, int mouse_x, int mouse_y )
{
	// Set up to read from the renderbuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		int window_width = glutGet(GLUT_WINDOW_WIDTH);
		int window_height = glutGet(GLUT_WINDOW_HEIGHT);
	 
		GLfloat color[4];
		   
		glReadPixels(mouse_x, window_height - mouse_y - 1, 1, 1, GL_RGBA, GL_FLOAT, color);

		if(color[0] == 1.0)
		{
			click0 = !click0;
			if(click0)	
				diffuse0 = vec4( 0.07568+1, 0.61424+1, 0.07568+1, 1.0 );
			else
				diffuse0 = vec4( 0.07568, 0.61424, 0.07568, 1.0 );
		}
		else if(color[1] == 1.0)
		{
			click1 = !click1; 
			if(click1)	
				diffuse1 = vec4( 0.61424+1, 0.04136+1, 0.04136+1, 1.0 );
			else
				diffuse1 = vec4( 0.61424, 0.04136, 0.04136, 1.0 );
		}
		else if(color[2] == 1.0)
		{
			click2 = !click2; 
			if(click2)	
				diffuse2 = vec4( 0.07568+1, 0.61424+1, 0.07568+1, 1.0 );
			else
				diffuse2 = vec4( 0.07568, 0.61424, 0.07568, 1.0 );
		}
			
	}
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{

	makeVertices( );

	cout << endl;
	cout << "INSTRUCTIONS FOR KEYS" << endl;
	cout << "'q' -  quits the program" << endl;
	cout << endl;
	cout << "'=' -  rotates camera counter-clockwise" << endl;
	cout << "'-' -  rotates camera clockwise" << endl;
	cout << "'r' -  increases orbit radius" << endl;
	cout << "'R' -  decreases orbit radius" << endl;
	cout << "'h' -  increases height of camera on cylinder" << endl;
	cout << "'H' -  decreases height of camera on cylinder" << endl;
	cout << "'a' -  toggles anti-aliasing" << endl;
	cout << endl;
	cout << "'z' -  resets all transformations" << endl;
	cout << endl;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "MODELS AND SHADING, YO." );
    glewExperimental=GL_TRUE; 
    glewInit(); 

    init();

	glutIdleFunc( idleMain );
    glutDisplayFunc( displayMain );
    glutReshapeFunc( reshapeMain );
    glutKeyboardFunc( keyboard );
	glutMouseFunc(mouseMain);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable( GL_MULTISAMPLE );

	// Run
    glutMainLoop();
    return 0;
}

