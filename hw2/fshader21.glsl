in vec4 vPosition, vColor;
out vec4 color;

void main()
{
    gl_Position = vPosition;
	color = vColor;
}
