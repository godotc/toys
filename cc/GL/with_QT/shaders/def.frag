#version 330 core

out vec4 fragcolor;
in vec3 outColor;
in vec2 TexCord;

// obtain texture pic
uniform sampler2D texture0;

void main()
{
	fragcolor = texture(texture0, TexCord)* vec4(1,1,0,0.5);
}
