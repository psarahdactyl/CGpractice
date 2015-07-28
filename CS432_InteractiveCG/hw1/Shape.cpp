#include "Shape.h"

Shape::Shape(vec2 *verts, int numPoints, GLenum mode, GLuint loc) 
{

	this->mode = mode;
	this->numPoints = numPoints;


	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(verts)*2*numPoints, verts, GL_STATIC_DRAW );

    // Initialize the vertex position attribute from the vertex shader
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

}

void Shape::draw()
{
    glBindVertexArray( vao );
    glDrawArrays( mode, 0, numPoints );    // draw the points

}
