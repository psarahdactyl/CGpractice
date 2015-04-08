// First new GL program
// Just makes a red triangle

#include "Angel.h"

const int triNumPoints = 3;
const int quadNumPoints = 4;
const int pentNumPoints = 5;

// Create a vertex array object
GLuint vao[3];

//--------------------------------------------------------------------------

void
init( void )
{
    // Specifiy the vertices for a triangle
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
		vec2( 0.3, 0.3 ), vec2( 0.2, 0.583 ), vec2( 0.45, 0.749 ), vec2( 0.7, 0.583 ), vec2( 0.6, 0.3 )
	   	};


    glGenVertexArrays( 3, vao );

    glBindVertexArray( vao[0] );
    

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(triVerts), triVerts, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );


    glBindVertexArray( vao[1] );

    GLuint buffer2;
    glGenBuffers( 1, &buffer2 );
    glBindBuffer( GL_ARRAY_BUFFER, buffer2 );
    glBufferData( GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW );


    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );


    glBindVertexArray( vao[2] );

    GLuint buffer3;
    glGenBuffers( 1, &buffer3 );
    glBindBuffer( GL_ARRAY_BUFFER, buffer3 );
    glBufferData( GL_ARRAY_BUFFER, sizeof(pentVerts), pentVerts, GL_STATIC_DRAW );

    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glBindVertexArray( vao[0] );
    glDrawArrays( GL_TRIANGLES, 0, triNumPoints );    // draw the points
    glBindVertexArray( vao[1] );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, quadNumPoints );    // draw the points
    glBindVertexArray( vao[2] );
    glDrawArrays( GL_TRIANGLE_FAN, 0, pentNumPoints );    // draw the points
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
