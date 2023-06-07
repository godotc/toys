#version 330 core
layout (location = 0 ) in vec3 vertex; 
layout (location = 1 ) in vec2 texcoords; 
									

out vec2 TexCoords;

uniform mat4 model = mat4(1.f);
uniform mat4 projection =mat4(1.f);


void main()
{
	TexCoords = texcoords;
	gl_Position = projection * model * vec4(vertex, 1.0 );
}

