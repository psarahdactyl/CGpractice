#version 120

varying vec4 color;
varying vec2 vTexCoord;

uniform int shading_type;
uniform sampler2D texture;

void main()
{
	gl_FragColor = color * (texture2D(texture, gl_PointCoord));
} 
