#version 330 core

in vec2 TexCoords;
in vec3 FragPos;

out vec4 color;

struct Light {
	vec3 position;
	float dist;
};

uniform float u_Time;

uniform sampler2D u_Texture;
uniform float u_TextureWidth;
uniform float u_TextureHeight;
uniform bool u_UseTexture;
uniform bool u_UseColorTexture;
uniform sampler2D u_DistortionTexture;
uniform bool u_UseDistortion;
uniform bool u_ScrollDistortionX;
uniform bool u_ScrollDistortionY;
uniform float u_DistortionSpeed;

uniform vec3 u_SpriteColor;
uniform float u_Alpha;
uniform float u_LayerAlpha;

uniform bool u_EnableLighting;
uniform float u_AmbientLighting;
#define NR_POINT_LIGHTS  10
uniform Light u_Light[NR_POINT_LIGHTS];

uniform bool u_Outline;
uniform float u_AspectRatio;

const float outlineSize = 1 / 25.0;
const float offset = 1.0 / 128.0;

void main()
{    
	vec4 result;
	if (u_UseTexture)
	{
		if (u_UseDistortion)
		{
			float waveX = 0.f;
			float waveY = 0.f;
			if (u_ScrollDistortionX)
				waveX = u_Time;
			if (u_ScrollDistortionY)
				waveY = u_Time;
			vec2 distortion = (texture(u_DistortionTexture, vec2(TexCoords.x + waveX, TexCoords.y + waveY)).rg * 2. - 1.) * u_DistortionSpeed;
			result = texture(u_Texture, vec2(TexCoords.x + distortion.x, TexCoords.y + distortion.y)) * vec4(1, 1, 1, u_Alpha);
		}
		else
		{
			result = texture(u_Texture, vec2(TexCoords.x, TexCoords.y)) * vec4(1, 1, 1, u_Alpha);
		}
		if (u_UseColorTexture)
		{
			if (result.a > 0)
			{
				result = vec4(u_SpriteColor, u_Alpha);
			}
		}
	}
	else
		result = vec4(u_SpriteColor, u_Alpha);

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

	// outline shader
	if (u_Outline)
	{
		if (u_UseTexture)
		{
			if (result.a <= 0.5)
			{
				float a = texture(u_Texture, vec2(TexCoords.x + offset, TexCoords.y)).a +
					texture(u_Texture, vec2(TexCoords.x, TexCoords.y - offset)).a +
					texture(u_Texture, vec2(TexCoords.x - offset, TexCoords.y)).a +
					texture(u_Texture, vec2(TexCoords.x, TexCoords.y + offset)).a;
				if (result.a < 1.0 && a > 0.0)
					result = vec4(1.0, 1.0, 1.0, 1.0);
			}
		}
		else
		{
		   float maxX = 1.0 - outlineSize;
		   float minX = outlineSize;
		   float maxY = maxX / u_AspectRatio;
		   float minY = minX / u_AspectRatio;

		   if (!(TexCoords.x < maxX && TexCoords.x > minX &&
			   TexCoords.y < maxY && TexCoords.y > minY))
			   result = vec4(1.0, 1.0, 1.0, 1.0);
		}
	}

	color = result * vec4(1, 1, 1, u_LayerAlpha);

}