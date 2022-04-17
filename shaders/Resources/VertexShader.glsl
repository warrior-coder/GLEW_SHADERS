#version 110

varying vec4 v_color;
 
void main()
{
	gl_Position = gl_ModelViewMatrix * gl_Vertex;
	v_color = gl_Color;
}