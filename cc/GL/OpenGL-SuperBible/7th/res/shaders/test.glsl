#shader vertex
#version 450 core

layout(location=0) in vec4 position;

out VS_Output
{
	vec4 color;
} vs_out;

uniform mat4 mv_mat;
uniform mat4 proj_mat;


void main(){
	gl_Position = proj_mat * mv_mat * position;
	vs_out.color = position * 2.f + vec4(0.5, 0.5, 0.5, 1);

}


#shader fragment
#version 450 core

in VS_Output
{
	vec4 color;
} fs_in;

out vec4 color;

void main(){
	color = fs_in.color;
}
