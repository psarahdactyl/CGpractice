attribute vec4 vPosition;
attribute vec3 vNormal;
varying vec4 color;

uniform mat4 model_view;
uniform mat4 projection;

uniform vec4 DiffuseProduct1;
uniform vec4 DiffuseProduct2;
uniform vec4 LightPosition1;
uniform vec4 LightPosition2;

void main()
{
	vec3 pos = (model_view * vPosition).xyz;

	vec3 L1 = normalize( (model_view*LightPosition1).xyz - pos );
    vec3 E1 = normalize( -pos );
	vec3 H1 = normalize( L1 + E1 );

	vec3 L2 = normalize( (model_view*LightPosition2).xyz - pos );
    vec3 E2 = normalize( -pos );
	vec3 H2 = normalize( L2 + E2 );

	vec3 N = normalize( model_view*vec4(vNormal, 0.0) ).xyz;

	float Kd1 = max( dot(L1, N), 0.0 );
	vec4 diffuse1 = Kd1*DiffuseProduct1;

	float Kd2 = max( dot(L2, N), 0.0 );
	vec4 diffuse2 = Kd2*DiffuseProduct2;

	gl_Position = projection * model_view * vPosition;
	color = diffuse1 + diffuse2;
	color.a = 1.0;
	color.c = 1.0;

}

