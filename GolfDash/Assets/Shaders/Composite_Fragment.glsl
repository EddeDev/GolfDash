#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D u_Texture;

vec3 ApplyVignette(vec3 color, float falloff)
{
	vec2 resolution = vec2(textureSize(u_Texture, 0));
	vec2 uv = gl_FragCoord.xy / resolution;
	vec2 coord = (uv - 0.5) * vec2(resolution.x / resolution.y, 1.0) * 2.0;
	float rf = sqrt(dot(coord, coord)) * falloff;
	float rf2_1 = rf * rf + 1.0;
	float e = 1.0 / (rf2_1 * rf2_1);
	return color * e;
}

void main()
{
	vec4 texColor = texture(u_Texture, Input.TexCoord);
	texColor.rgb = ApplyVignette(texColor.rgb, 0.45);
	o_Color = texColor;
}