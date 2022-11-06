#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, Input.TexCoord);
	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
}