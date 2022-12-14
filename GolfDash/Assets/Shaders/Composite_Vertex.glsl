#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

struct VertexOutput
{
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;

void main()
{
	Output.TexCoord = a_TexCoord;

	gl_Position = vec4(a_Position, 1.0);
}