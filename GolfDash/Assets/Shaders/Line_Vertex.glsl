#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) out VertexOutput Output;

struct Uniforms
{
	mat4 ViewProjectionMatrix;
};

layout(location = 0) uniform Uniforms u_Uniforms;

void main()
{
	Output.Color = a_Color;

	gl_Position = u_Uniforms.ViewProjectionMatrix * vec4(a_Position, 1.0);
}