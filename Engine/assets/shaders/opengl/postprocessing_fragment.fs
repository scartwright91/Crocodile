#version 330 core
in vec2 TexCoords;
in vec2 ScreenCoords;

out vec4  color;

uniform sampler2D u_Scene;

uniform bool u_Greyscale;
uniform bool u_Wavey;

uniform bool u_BeginScene;
uniform bool u_EndScene;
uniform float u_TransitionCounter;

uniform bool u_FadeTransition;
uniform bool u_DiamondTransition;

void main()
{
	color = texture(u_Scene, TexCoords);

	// greyscale
	if (u_Greyscale)
	{
		float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
		color = vec4(average, average, average, 1.0);
	}

	// wave effect
	if (u_Wavey)
	{
		vec2 wavecoord = TexCoords;
		wavecoord.x += sin(wavecoord.y * 4*2*3.14159 + 100) / 100;
		color = texture(u_Scene, wavecoord);
	}

	// fade-in/out transitions
	if (u_EndScene)
	{
		if (u_FadeTransition)
		{
			color = mix(color, vec4(vec3(0.0f), 1.0), 1.0f - u_TransitionCounter);
		}
		else if (u_DiamondTransition)
		{
			float xFraction = fract(ScreenCoords.x / 50.f);
			float yFraction = fract(ScreenCoords.y / 50.f);
			float xDistance = abs(xFraction - 0.5);
			float yDistance = abs(yFraction - 0.5);
			if (xDistance + yDistance + TexCoords.x + TexCoords.y > u_TransitionCounter * 4.0f) {
				color = vec4(1.0, 0.0, 1.0, 1.0);
			}
		}
	} else if (u_BeginScene)
	{
		if (u_FadeTransition)
		{
			color = mix(color, vec4(vec3(0.0f), 1.0), u_TransitionCounter);
		}
		else if (u_DiamondTransition)
		{
			float xFraction = fract(ScreenCoords.x / 50.f);
			float yFraction = fract(ScreenCoords.y / 50.f);
			float xDistance = abs(xFraction - 0.5);
			float yDistance = abs(yFraction - 0.5);
			if (xDistance + yDistance + TexCoords.x + TexCoords.y > (1 - u_TransitionCounter) * 4.0f) {
				color = vec4(1.0, 0.0, 1.0, 1.0);
			}
		}
	}

	color = color;

}