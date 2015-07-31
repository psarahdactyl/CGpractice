attribute vec4 vPosition, vColor;
varying vec4 color;
varying vec2 vTexCoord;

void main()
{
	vTexCoord = vec2(vPosition.x, vPosition.y);

	gl_Position = vPosition;
	color = vColor;
	color.a = vColor[3];

}

