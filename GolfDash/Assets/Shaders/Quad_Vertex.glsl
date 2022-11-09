#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in float a_Wave;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
	float Wave;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat float v_TexIndex;

struct Uniforms
{
	mat4 ViewProjectionMatrix;
};

layout(location = 0) uniform Uniforms u_Uniforms;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TilingFactor = a_TilingFactor;
	Output.Wave = a_Wave;
	v_TexIndex = a_TexIndex;

	gl_Position = u_Uniforms.ViewProjectionMatrix * vec4(a_Position, 1.0);
}