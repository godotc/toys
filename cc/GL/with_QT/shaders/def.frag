#version 330 core

out vec4 fragcolor;
in vec3 outColor;
in vec2 TexCord;

// obtain texture pic
uniform sampler2D textureArch;
uniform sampler2D textureBrick;

void main()
{
	fragcolor = mix(
		texture(textureArch, TexCord),
		texture(textureBrick, TexCord),
		0.5);

}
