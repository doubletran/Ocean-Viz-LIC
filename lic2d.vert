#version 330 compatibility 

const vec3 LIGHTPOS = normalize( vec3( 5., 10., 10. ));

out vec3 vL;
out vec3 vN;
uniform sampler2D FlowUnit;

void
main( void )
{

	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec2 st = gl_MultiTexCoord0.st;
	vec3 norm = normalize (gl_NormalMatrix * gl_Normal);
	vN = norm;
	vec3 vert = gl_Vertex.xyz;
	vec4 v = texture(FlowUnit, st);

	
	if ((v.a == 0.)  ){
		vert += normalize(gl_Normal) * 0.05;
	}
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vert, 1.);
	vec3 ECposition = vec3( gl_ModelViewMatrix * vec4(vert, 1.) );
	vL = LIGHTPOS - ECposition.xyz;
	
}

