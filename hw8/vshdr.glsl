attribute vec4 vPosition;
attribute vec3 vNormal;
varying vec4 color;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL1;

uniform mat4 model_view;
uniform mat4 projection;

uniform vec4 DiffuseProduct1;
uniform vec4 AmbientProduct1;
uniform vec4 SpecularProduct1;
uniform vec4 LightPosition1;

uniform float Shininess;

void main()
{
	vec3 pos = (model_view * vPosition).xyz;

	vec3 L1 = normalize( LightPosition1.xyz - pos );
    vec3 E = normalize( -pos );
	vec3 H1 = normalize( L1 + E );

	vec3 N = normalize( model_view*vec4(vNormal, 0.0) ).xyz;

	//DIFFUSE
	float Kd1 = max( dot(L1, N), 0.0 );
	vec4 diffuse1 = Kd1*DiffuseProduct1;

	//SPECULAR
	float Ks1 = pow( max(dot(N, H1), 0.0), Shininess );
	vec4 specular1 = Ks1 * SpecularProduct1;
	if( dot(L1, N) < 0.0 ) specular1 = vec4(0.0, 0.0, 0.0, 1.0);

	//AMBIENT
	vec4 ambient1 = AmbientProduct1;

	color = diffuse1 + ambient1 + specular1;
	color.a = 1.0;

	gl_Position = projection * model_view * vPosition;

}

