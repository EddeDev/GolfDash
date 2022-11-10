#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D u_Texture;

layout(location = 1) uniform float u_Time;
layout(location = 2) uniform float u_Wave;

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
	vec2 texCoord = Input.TexCoord;
	
	// Wave effect
	texCoord.y = (texCoord.y + u_Wave * sin(u_Time + 10.0 * texCoord.x));

	vec3 color = texture(u_Texture, texCoord).rgb;

	color = ApplyVignette(color, 0.45);
	o_Color = vec4(color, 1.0);
}