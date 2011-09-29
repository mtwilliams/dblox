uniform sampler2DArray Textures;

void main()
{
	vec4 Color;
	Color = texture2DArray(Textures, vec3(gl_TexCoord[0].xyz));
	
	gl_FragColor = Color;
}