uniform float Length;
uniform sampler2D TexUnit;
uniform sampler2D uFlowUnit;
uniform sampler2D WorldTexUnit;

void
main( void )
{

	vec2 v;
	int LengthP;
	//float TwoOverRes = 2. / 512.;
 	ivec2 res = textureSize( TexUnit, 0 );
	float OneOverNum = 1./(Length+Length+1.);
	vec3 color;
	
	// starting location:

	vec2 st = gl_TexCoord[0].st;
	v = texture(uFlowUnit, st).xy; 
   v *= 1./res;

	color = vec3( texture2D( TexUnit, st ) );
	st = gl_TexCoord[0].st;
	LengthP = int( Length );

	for( int i = 0; i < LengthP; i++ )
	{
		st += v;
		st = clamp( st, 0., 1. );
		color += vec3( texture2D( TexUnit, st ) );
	}
	st = gl_TexCoord[0].st;
	LengthP = int( Length );
	for( int i = 0; i < LengthP; i++ )
	{
		st -= v;
		st = clamp( st, 0., 1. );
		color += vec3( texture2D( TexUnit, st ) );
	}
		st = gl_TexCoord[0].st;
		vec3 worldTexColor = vec3(texture2D (WorldTexUnit, st));
		
	color *= OneOverNum;
		color.z = 0.7;

	if ((v.x == 0.) && (v.y == 0.)){
		gl_FragColor = vec4(0., 0., 0., 1.);

	}
	else{
   gl_FragColor = vec4( color, 1.);
	}
	
}
