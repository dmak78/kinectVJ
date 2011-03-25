/*************************************************************
GLSL Demo - Per-pixel cube mapping vertex shader
(c) 2005 Antoine Bouthors, EVASION
*************************************************************/

varying vec3 normal;
varying vec3 view;

void main()
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	normal = gl_NormalMatrix * gl_Normal;
	view = -pos.xyz;
	gl_Position = ftransform();
}