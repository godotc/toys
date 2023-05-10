#version 330 core

uniform vec4  outColor;
out vec4 fragcolor;

void main()
{
	fragcolor = outColor;
}
