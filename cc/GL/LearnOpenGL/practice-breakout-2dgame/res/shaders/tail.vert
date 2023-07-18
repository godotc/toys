#version 330 core
layout(location = 0) in vec4 vertex; // 2 pos 2 texCoords

out vec2 TexCoords;
out vec4 ParticleColor;

unifrom mat4 projection;
unifrom vec2 offset;
unifrom vec4 color;


void main()
{
	float scale = 10.f;
	TexCoords = Color;
	ParticleColor = color;
	gl_Position = projection * vec4( (vertex.xy * scale) + offset,
									0.f, 1.f);
}
