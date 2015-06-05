attribute vec4 vPosition;
attribute vec3 vNormal;
varying vec4 color;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL1;
varying vec3 fL2;

uniform mat4 model_view;
uniform mat4 projection;

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

void main()
{
	if( shading_type == 1 ) 
	{
		vec3 pos = (model_view * vPosition).xyz;

		vec3 L1 = normalize( (model_view*LightPosition1).xyz - pos );
    	vec3 E = normalize( -pos );
		vec3 H1 = normalize( L1 + E );

		vec3 L2 = normalize( (model_view*LightPosition2).xyz - pos );
		vec3 H2 = normalize( L2 + E );

		vec3 N = normalize( model_view*vec4(vNormal, 0.0) ).xyz;

		//DIFFUSE
		float Kd1 = max( dot(L1, N), 0.0 );
		vec4 diffuse1 = Kd1*DiffuseProduct1;

		float Kd2 = max( dot(L2, N), 0.0 );
		vec4 diffuse2 = Kd2*DiffuseProduct2;

		color = diffuse1 + diffuse2;
		color.a = 1.0;

	}
	else if( shading_type == 0 )
	{
		fN = vNormal;
		fE = vPosition.xyz;
		fL1 = LightPosition1.xyz;
		fL2 = LightPosition2.xyz;

		if( LightPosition1.w != 0.0 ) {
			fL1 = LightPosition1.xyz - vPosition.xyz;
		}
		else if( LightPosition2.w != 0.0 ) {
			fL2 = LightPosition2.xyz - vPosition.xyz;
		}
	}
	else if( shading_type == 2 )
	{
		color = vec4(vNormal, 1.0);
	}

	gl_Position = projection * model_view * vPosition;

}
