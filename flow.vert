
#version 330 compatibility 

 
out vec2    vST; 
 
void  main( void ) 
{ 
 vST = gl_MultiTexCoord0.st; 
 gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex; 
} 