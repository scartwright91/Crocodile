#version 330 core

in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform vec3 u_Color;
uniform float u_Alpha;
uniform float u_LayerAlpha;

out vec4 color;

void main()
{
	vec4 result;
	if (u_UseTexture)
	{
		result = texture(u_Texture, vec2(TexCoords.x, TexCoords.y)) * vec4(1, 1, 1, u_Alpha);
	}
	else
	{
		result = vec4(u_Color, u_Alpha);
	}
	color = result * vec4(1, 1, 1, u_LayerAlpha);
}
