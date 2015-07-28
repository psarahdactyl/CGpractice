#include "Shape.h"

Shape::Shape(GLfloat x, GLfloat y, GLfloat r, int n, GLenum mode) 
{
	this->mode = mode;
	setup();
}

void Shape::setup()
{
	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
	vec2 *points = &Shape::coords[0];
    glBufferData( GL_ARRAY_BUFFER, sizeof(Shape::coords)*2*numPoints, points, GL_STATIC_DRAW );

    // Initialize the vertex position attribute from the vertex shader
    glEnableVertexAttribArray( location );
    glVertexAttribPointer( location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
}


void Shape::draw()
{
    glBindVertexArray( vao );
    glDrawArrays( mode, 0, numPoints );    // draw the points

}


