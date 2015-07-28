varying vec4 color;

uniform int shading_type;

uniform vec4 DiffuseProduct1;
uniform vec4 DiffuseProduct2;
uniform vec4 AmbientProduct1;
uniform vec4 AmbientProduct2;
uniform vec4 SpecularProduct1;
uniform vec4 SpecularProduct2;
uniform vec4 LightPosition1;
uniform vec4 LightPosition2;

uniform float Shininess;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL1;
varying vec3 fL2;

void main()
{
	if( shading_type == 0 )
	{
		vec3 N = normalize( fN );
		vec3 E = normalize( fE );
		vec3 L1 = normalize( fL1 );
		vec3 L2 = normalize( fL2 );
		vec3 H1 = normalize( L1 + E );
		vec3 H2 = normalize( L2 + E );

		// AMBIENT
		vec4 ambient1 = AmbientProduct1;

		vec4 ambient2 = AmbientProduct2;


		// DIFFUSE
		float Kd1 = max( dot(L1, N), 0.0 );
		vec4 diffuse1 = Kd1 * DiffuseProduct1;

		float Kd2 = max( dot(L2, N), 0.0 );
		vec4 diffuse2 = Kd2*DiffuseProduct2;


		// SPECULAR
		float Ks1 = pow( max(dot(N, H1), 0.0), Shininess );
		vec4 specular1 = Ks1 * SpecularProduct1;

		float Ks2 = pow( max(dot(N, H2), 0.0), Shininess );
		vec4 specular2 = Ks2 * SpecularProduct2;


		// discard the specular highlight if the light’s behind the vertex
		if( dot(L1, N) < 0.0 )
				specular1 = vec4(0.0, 0.0, 0.0, 1.0);
		else if( dot(L2, N) < 0.0 ) 
				specular2 = vec4(0.0, 0.0, 0.0, 1.0);

		gl_FragColor = diffuse1 + diffuse2 + ambient1 + ambient2 + specular1 + specular2;
		gl_FragColor.a = 1.0;
	}

	else if( shading_type == 1 || shading_type == 2 )
		gl_FragColor = color;
} 
