#version 330 core

in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform vec3 u_SpriteColor;
uniform float u_Alpha;

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
		result = vec4(u_SpriteColor, u_Alpha);
	}
	color = result;
}
