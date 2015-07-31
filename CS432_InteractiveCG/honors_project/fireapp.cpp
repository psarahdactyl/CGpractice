// Sarah Kushner
// CS 432
// Extra Credit / Honors Assignment

#define MAX_NUM_PARTICLES 16000
#define MIN_NUM_PARTICLES 500
#define INITIAL_NUM_PARTICLES 2000
#define INITIAL_SPEED 1.0
#define NUM_COLORS 8
#define FALSE 0
#define TRUE 1

#include "Angel.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// RGBA colors
static vec4 vertex_colors[8] = {
	vec4( 0.63, 0.63, 0.63, 1.0 ), // grey
	vec4( 1.0, 0.0, 0.0, 1.0 ), // red
	vec4( 1.0, 1.0, 0.0, 1.0 ), // yellow
	vec4( 1.0, 0.49, 0.31, 1.0 ), // coral
	vec4( 1.0, 0.27, 0.0, 1.0 ), // orange red
	vec4( 1.0, 0.84, 0.0, 1.0 ), // gold
	vec4( 1.0, 0.65, 0.0, 1.0 ), // orange
	vec4( 1.0, 0.55, 0.0, 1.0 ), // dark orange
};

vec4 points[MAX_NUM_PARTICLES];
vec4 colors[MAX_NUM_PARTICLES];

GLuint program;
GLuint vao;
mat4 model_view;
mat4 projection;

GLuint buffer;
GLuint loc, loc2;
GLuint model_view_loc, projection_loc;

/* particle struct */

typedef struct particle {
    vec4 color;
	float life;
	float original_life;
	float size;
	float original_size;
    vec4 position;
    vec4 original_position;
    vec4 velocity;
	vec4 original_velocity;
    float mass;
} particle;

particle particles[MAX_NUM_PARTICLES];      

int present_time;
int last_time;
int num_particles = INITIAL_NUM_PARTICLES;
float speed = INITIAL_SPEED;
bool elastic = FALSE;            /* restitution off */
bool repulsion = FALSE;          /* repulsion off */
float d2[MAX_NUM_PARTICLES][MAX_NUM_PARTICLES];   /* array for interparticle distances */

// Texture
int texture;

// Windows
int mainWin;


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

void createParticle( int i )
{
        particles[i].mass = 1.0;
		int col = i % NUM_COLORS;
        particles[i].color = vertex_colors[col];
        for ( int j = 0; j < 3; j++ ) {
			if(j == 0) {
            	particles[i].original_position[j] =
                	( ( float ) rand() / RAND_MAX ) - 0.5;
            	particles[i].position[j] = particles[i].original_position[j];
			}
			if(j == 1 or j == 2) {
            	particles[i].original_position[j] =
                	(( ( float ) rand() / RAND_MAX ) / 2.0 ) - 1.3;
            	particles[i].position[j] = particles[i].original_position[j];
			}
            particles[i].original_velocity[j] =
                speed * 2.0 * ( ( float ) rand() / RAND_MAX ) - 1.0;
			particles[i].velocity[j] = particles[i].original_velocity[j];
        }
        particles[i].position[3] = 1.0;
		particles[i].original_life = 1.0 * ( ( float ) rand() / RAND_MAX ); 
		particles[i].life = particles[i].original_life;
		particles[i].original_size = 8.0;
		particles[i].size = particles[i].original_size;

}

void init( )
{

    // Set up particles with random velocities, semi-random positions and colors
    for ( int i = 0; i < num_particles; i++ ) {
			createParticle( i );
    }

	// Initialization
	GLuint vao, buffer, tex;

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

	// Create and initialize a texture object
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	int x, y, comp;
	unsigned char *my_texels = stbi_load("particle.png", &x, &y, &comp, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, my_texels);
	glTexEnvi(GL_POINT_SPRITE_ARB,GL_COORD_REPLACE_ARB,GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 	glActiveTexture( GL_TEXTURE0 );

	stbi_image_free( my_texels );

	// Load array into buffer
	loadBuffer();

	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "vshdr.glsl", "fshdr.glsl" );
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

	glUniform1i( glGetUniformLocation(program, "texture"), 0 );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );

}

//----------------------------------------------------------------------------

float
forces( int i, int j )
{
    int k;
    float force = 0.0;
    if ( repulsion )
        for ( k = 0; k < num_particles; k++ ) { /* repulsive force */
            if ( k != i )
                force +=
                    0.001 * ( particles[i].position[j] -
                              particles[k].position[j] ) / ( 0.001 +
                                                             d2[i][k] );
        }
    return ( force );
}

//----------------------------------------------------------------------------

void displayMain( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

    for ( int i = 0; i < num_particles; i++ ) {
        colors[i] = particles[i].color;
        particles[i].position[3] = 1.0;
        points[i] = particles[i].position;
    }

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    glDrawArrays( GL_POINTS, 0, num_particles );
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idleMain( void )
{
    int i, j;
    float dt;
    present_time = glutGet( GLUT_ELAPSED_TIME );
    dt = 0.03;
    for ( i = 0; i < num_particles; i++ ) {
        for ( j = 1; j < 2; j++ ) {
            particles[i].position[j] += dt * particles[i].velocity[j];
            particles[i].velocity[j] += dt * forces( i, j ) / particles[i].mass;
        }
		particles[i].life -= dt;
		if(particles[i].life > 0) {
			float age_ratio = particles[i].life / particles[i].original_life;
			particles[i].color[3] = age_ratio;
		}
		else {
			createParticle( i );
		}

    }
    last_time = present_time;
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	    case 'q':
	        exit( EXIT_SUCCESS );
	     	break;
		case 'm':
			if(num_particles < MAX_NUM_PARTICLES)
	            num_particles *= 2;
			break;
	
		case 'n':
			if(num_particles > MIN_NUM_PARTICLES)
				num_particles /= 2;
			break;
	
		case 'f':
			speed *= 1.2;
			break;
	
		case 's':
			speed /= 1.2;
			break;
	
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void mainMenu( int id )
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

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 500, 500 );

	// Main Window
    mainWin = glutCreateWindow( "FIRE IN MY ROOM" );
    glewExperimental=GL_TRUE; 
    glewInit(); 

	glutCreateMenu( mainMenu ); // Set up menu
	glutAddMenuEntry ( "Stop Fire", 1 );
	glutAddMenuEntry( "Start Fire", 2 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );


    init();
    glPointSize( particles[0].size );

	cout << "Keys:" << endl;
    cout <<  "'m'  -  more particles" << endl;
    cout <<  "'n'  -  fewer particles" << endl;
    cout <<  "'f'  -  faster particles" << endl;
    cout <<  "'s'  -  slower particles" << endl;

	glutIdleFunc( idleMain );
    glutDisplayFunc( displayMain );
    glutKeyboardFunc( keyboard );
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Run
    glutMainLoop();
    return 0;
}

