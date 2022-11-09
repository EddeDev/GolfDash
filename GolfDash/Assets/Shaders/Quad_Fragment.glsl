#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
	float Wave;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat float v_TexIndex;

layout(binding = 0) uniform sampler2D u_Textures[32];

layout(location = 1) uniform float u_Time;

void main()
{
	vec4 color = Input.Color;

	vec2 texCoord = Input.TexCoord * Input.TilingFactor;
	if (Input.Wave >= 0.5)
		texCoord.y = texCoord.y + 0.07 * sin(u_Time + 10.0 * texCoord.x);

	color *= texture(u_Textures[int(v_TexIndex)], texCoord);

	if (color.a == 0.0)
		discard;

	o_Color = color;
}