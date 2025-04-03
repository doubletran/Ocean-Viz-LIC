 
#version 330 compatibility

uniform sampler2D   uFlowUnit; 
in vec2             vST; 
void 
main( void ) 
{ 
    vec2 st = vST;
    vec3 color = vec3( texture2D( uFlowUnit, st ) );
    gl_FragColor = vec4 (color, 1.);
} 