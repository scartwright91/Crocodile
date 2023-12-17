#version 330 core

in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform vec3 u_Color;
uniform float u_Alpha;
uniform float u_LayerAlpha;

struct Light {
	vec3 position;
	float dist;
};
uniform bool u_EnableLighting;
uniform float u_AmbientLighting;
#define NR_POINT_LIGHTS  10
uniform Light u_Light[NR_POINT_LIGHTS];

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

	// apply lighting
	if (u_EnableLighting)
	{
		float light = 0.0f;
		for(int i = 0; i < NR_POINT_LIGHTS; i++)
		{
			float d = distance(u_Light[i].position, FragPos);
			if (d < u_Light[i].dist)
				light += (u_Light[i].dist - d) / u_Light[i].dist;
		}
		result.rgb *= max(min(1.0, light), u_AmbientLighting);
	}

	color = result * vec4(1, 1, 1, u_LayerAlpha);
}
