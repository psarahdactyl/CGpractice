// First new GL program
// Just makes red shapes

#include "Angel.h"
#include "Shape.h"

Shape *shapes[3];

//--------------------------------------------------------------------------

void
init( void )
{
    // Specify the vertices for a triangle
    vec2 triVerts[3] = {
        vec2( -0.75, 0.1 ), vec2( -0.5, 0.5 ), vec2( -0.25, 0.1 )
    };

	// Vertices for a square
	vec2 quadVerts[4] = {
		vec2( -0.25, -0.1 ), vec2( 0.25, -0.1 ), vec2( -0.25, -0.6 ), 
		vec2( 0.25, -0.6 )
	};

	// Vertices for a pentagon
	vec2 pentVerts[5] = {
		vec2( 0.3, 0.3 ), vec2( 0.2, 0.583 ), vec2( 0.45, 0.749 ), 
		vec2( 0.7, 0.583 ), vec2( 0.6, 0.3 )
	};

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
	glUseProgram( program );
    GLuint loc = glGetAttribLocation( program, "vPosition" );

	shapes[0] = new Shape(triVerts, 3, GL_TRIANGLES, loc);
	shapes[1] = new Shape(quadVerts, 4, GL_TRIANGLE_STRIP, loc);
	shapes[2] = new Shape(pentVerts, 5, GL_TRIANGLE_FAN, loc);

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
	for(int i = 0; i < 3; i++)
	{
		shapes[i]->draw();
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
    glutInitWindowSize( 512, 512 );

    glutCreateWindow( "Red Shapes" );
    glewExperimental=GL_TRUE; 
    glewInit();    
    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}

