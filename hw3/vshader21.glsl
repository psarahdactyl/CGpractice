attribute vec4 vPosition, vColor;
varying vec4 color;

void main()
{
    gl_Position = vPosition;
	color = vColor;
}

