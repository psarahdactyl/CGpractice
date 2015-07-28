attribute vec4 vPosition, vColor;
varying vec4 color;

uniform mat4 scale;
uniform mat4 rotatex;
uniform mat4 rotatey;
uniform mat4 rotatez;
uniform mat4 translate;

void main()
{

	gl_Position = scale * rotatex * rotatey * rotatez * translate * vPosition;
	color = vColor;

}

