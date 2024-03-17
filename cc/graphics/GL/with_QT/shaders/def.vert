#version 330 core

layout(location =0) in vec3 aPos;
layout(location =1) in vec3 aColor;
layout(location =2) in vec2 aTex;

out vec3 outColor;
out vec2 TexCord;

void main()
{
	outColor = aColor;
	TexCord  = aTex;
	gl_Position = vec4(aPos, 1);
}
