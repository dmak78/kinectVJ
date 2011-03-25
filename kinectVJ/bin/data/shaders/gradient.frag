/*************************************************************
GLSL Demo - Per-pixel cube mapping fragment shader
(c) 2005 Antoine Bouthors, EVASION
*************************************************************/

uniform samplerCube TexCube;
varying vec3 normal;
varying vec3 view;


void main()
{
	vec3 Nnormal = normalize( normal );
	vec3 Nview = normalize( view );
	vec3 reflex = reflect( Nview, Nnormal );
	vec3 FinalTexCoord = mat3(gl_TextureMatrix[0]) * reflex; 
	vec4 color = textureCube( TexCube, FinalTexCoord );

	gl_FragColor = color;
}