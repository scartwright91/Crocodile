#include "Application.h"

namespace Crocodile
{
	Application *Application::s_Instance = nullptr;

	Application::Application(const char *name, unsigned int width, unsigned height) : window(name, false, width, height)
	{
		s_Instance = this;

		init();
		loadShaders();
		loadTextures();
		scene = new s2d::Scene(&window, &resourceManager);
	}

	Application::~Application()
	{
		delete scene;
	}

	void Application::run()
	{
#ifdef CROCODILE_EMSCRIPTEN
		std::function<void()> mainLoop = [&]()
		{
			clock.tick(60);
			window.beginRender();
			update(clock.deltaTime);
			scene->update(clock.deltaTime);
			render();
			window.endRender();
		};
		emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
		while (!window.closed())
		{
			clock.tick(60);
			window.beginRender();
			update(clock.deltaTime);
			scene->update(clock.deltaTime);
			render();
			window.endRender();
		}
#endif
	}

	void Application::render()
	{
		scene->render();
	}

	void Application::init() const
	{
#ifndef CROCODILE_EMSCRIPTEN
		glewInit();
#endif
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Application::loadShaders()
	{
		std::string vertexCode;
		std::string fragmentCode;

		// sprite shader
#ifdef CROCODILE_EMSCRIPTEN
		vertexCode = R"(#version 300 es
precision highp float;

in vec4 vertex;

out vec2 TexCoords;
out vec3 FragPos;

// vertix position calculation
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// texture coordination calculation
uniform float u_NumberOfRows;
uniform float u_NumberOfCols;
uniform vec2 u_TextureOffset;
uniform bool u_FlipX;
uniform bool u_FlipY;

void main()
{

	if (u_NumberOfRows == 1.0 && u_NumberOfCols == 1.0)
		TexCoords = vertex.zw;
	else
		TexCoords = vec2(vertex.z / u_NumberOfCols + u_TextureOffset.x, vertex.w / u_NumberOfRows + u_TextureOffset.y);

	if (u_FlipX)
		TexCoords.x = 1.f - TexCoords.x;
	if (u_FlipY)
		TexCoords.y = 1.f - TexCoords.y;

	FragPos = vec3(u_Model * vec4(vertex.xy, 0.0, 1.0));
	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
}
		)";

		fragmentCode = R"(#version 300 es
precision mediump float;

in vec2 TexCoords;
in vec3 FragPos;

out vec4 color;

struct Light {
	vec3 position;
	float dist;
};

uniform float u_Time;

uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform sampler2D u_DistortionTexture;
uniform bool u_UseDistortion;
uniform bool u_ScrollDistortionX;
uniform bool u_ScrollDistortionY;
uniform float u_DistortionSpeed;

uniform vec3 u_SpriteColor;
uniform float u_Alpha;

uniform bool u_EnableLighting;
uniform float u_AmbientLighting;
#define NR_POINT_LIGHTS  10
uniform Light u_Light[NR_POINT_LIGHTS];

uniform bool u_Outline;
uniform float u_AspectRatio;

const float outlineSize = 1.0 / 25.0;
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

	}
	else
		result = vec4(u_SpriteColor, u_Alpha);

	// apply lighting
	if (u_EnableLighting)
	{
		float light = 0.0f;
		for(int i = 0; i < NR_POINT_LIGHTS; i++)
			light += u_Light[i].dist / (distance(u_Light[i].position, FragPos));
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
					result = vec4(1.0, 0.0, 0.0, 1.0);
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
			   result = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}

	color = result;

}
		)";
#else
		vertexCode = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec3 FragPos;

// vertix position calculation
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// texture coordination calculation
uniform float u_NumberOfRows;
uniform float u_NumberOfCols;
uniform vec2 u_TextureOffset;
uniform bool u_FlipX;
uniform bool u_FlipY;

void main()
{

	if (u_NumberOfRows == 1.0 && u_NumberOfCols == 1.0)
		TexCoords = vertex.zw;
	else
		TexCoords = vec2(vertex.z / u_NumberOfCols + u_TextureOffset.x, vertex.w / u_NumberOfRows + u_TextureOffset.y);

	if (u_FlipX)
		TexCoords.x = 1 - TexCoords.x;
	if (u_FlipY)
		TexCoords.y = 1 - TexCoords.y;

	FragPos = vec3(u_Model * vec4(vertex.xy, 0.0, 1.0));
	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
}
)";
		fragmentCode = R"(
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
uniform bool u_UseTexture;
uniform sampler2D u_DistortionTexture;
uniform bool u_UseDistortion;
uniform bool u_ScrollDistortionX;
uniform bool u_ScrollDistortionY;
uniform float u_DistortionSpeed;

uniform vec3 u_SpriteColor;
uniform float u_Alpha;

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

	}
	else
		result = vec4(u_SpriteColor, u_Alpha);

	// apply lighting
	if (u_EnableLighting)
	{
		float light = 0.0f;
		for(int i = 0; i < NR_POINT_LIGHTS; i++)
			light += u_Light[i].dist / (distance(u_Light[i].position, FragPos));
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
					result = vec4(1.0, 0.0, 0.0, 1.0);
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
			   result = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}

	color = result;

}
		)";
#endif
		resourceManager.loadShaderFromString(vertexCode, fragmentCode, "sprite");
		printf("Compiled Sprite Shader \n");

#ifdef CROCODILE_EMSCRIPTEN
		vertexCode = R"(#version 300 es

precision highp float;

in vec4 vertex;

out vec4 particleColor;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec2 u_Offset;
uniform float u_Scale;

uniform vec3 u_Color;
uniform float u_Alpha;

void main()
{
	particleColor = vec4(u_Color, u_Alpha);
	gl_Position = u_Projection * u_View * u_Model * vec4((vertex.xy * u_Scale) + u_Offset, 0.0, 1.0);
}
		)";

		fragmentCode = R"(#version 300 es

precision mediump float;

in vec4 particleColor;
out vec4 color;

void main()
{
	color = particleColor;
}  
		)";
#else
		// particle shader
		vertexCode = R"(
#version 330 core

layout (location = 0) in vec4 vertex;

out vec4 particleColor;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec2 u_Offset;
uniform float u_Scale;

uniform vec3 u_Color;
uniform float u_Alpha;

void main()
{
	particleColor = vec4(u_Color, u_Alpha);
	gl_Position = u_Projection * u_View * u_Model * vec4((vertex.xy * u_Scale) + u_Offset, 0.0, 1.0);
}
		)";
		fragmentCode = R"(
#version 330 core

in vec4 particleColor;
out vec4 color;

void main()
{
	color = particleColor;
}  
		)";
#endif
		resourceManager.loadShaderFromString(vertexCode, fragmentCode, "particle");
		printf("Compiled Particle Shader \n");

#ifdef CROCODILE_EMSCRIPTEN
		// text shader
		vertexCode = R"(#version 300 es

precision highp float;

in vec4 vertex;

out vec2 TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_View * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}
		)";

		fragmentCode = R"(#version 300 es

precision mediump float;

in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;
uniform float u_Alpha;

void main()
{    
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Text, TexCoords).r);
	color = vec4(u_TextColor, u_Alpha) * sampled;
}
		)";
#else
		// text shader
		vertexCode = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_View * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}
		)";
		fragmentCode = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;
uniform float u_Alpha;

void main()
{    
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Text, TexCoords).r);
	color = vec4(u_TextColor, u_Alpha) * sampled;
}
		)";
#endif
		resourceManager.loadShaderFromString(vertexCode, fragmentCode, "text");
		printf("Compiled Text Shader \n");

		// postprocessing
#ifdef CROCODILE_EMSCRIPTEN
		vertexCode = R"(#version 300 es

precision highp float;

in vec2 aPos;
in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 ScreenCoords;

uniform float u_DeltaTime;
uniform float u_ScreenWidth;
uniform float u_ScreenHeight;

uniform bool u_ScreenShake;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f); 
	TexCoords = aTexCoords;
	vec3 ndc = gl_Position.xyz / gl_Position.w;
	vec2 viewportCoords = ndc.xy * 0.5f + 0.5f;
	ScreenCoords = viewportCoords * vec2(viewportCoords * vec2(u_ScreenWidth, u_ScreenHeight));

	if (u_ScreenShake)
	{
		float strength = 0.005f;
		gl_Position.x += cos(u_DeltaTime * 100.f) * strength;
		gl_Position.y += cos(u_DeltaTime * 150.f) * strength;
	}

}
		)";

		fragmentCode = R"(#version 300 es

precision mediump float;

in vec2 TexCoords;
in vec2 ScreenCoords;

out vec4 color;

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
		wavecoord.x += sin(wavecoord.y * 4.f*2.f*3.14159 + 100.f) / 100.f;
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
			float xDistance = abs(xFraction - 0.5f);
			float yDistance = abs(yFraction - 0.5f);
			if (xDistance + yDistance + TexCoords.x + TexCoords.y > u_TransitionCounter * 4.f) {
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
			float xDistance = abs(xFraction - 0.5f);
			float yDistance = abs(yFraction - 0.5f);
			if (xDistance + yDistance + TexCoords.x + TexCoords.y > (1.f - u_TransitionCounter) * 4.f) {
				color = vec4(1.0, 0.0, 1.0, 1.0);
			}
		}
	}

}
		)";
#else
		vertexCode = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 ScreenCoords;

uniform float u_DeltaTime;
uniform float u_ScreenWidth;
uniform float u_ScreenHeight;

uniform bool u_ScreenShake;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f); 
	TexCoords = aTexCoords;
	vec3 ndc = gl_Position.xyz / gl_Position.w;
	vec2 viewportCoords = ndc.xy * 0.5 + 0.5;
	ScreenCoords = viewportCoords * vec2(viewportCoords * vec2(u_ScreenWidth, u_ScreenHeight));

	if (u_ScreenShake)
	{
		float strength = 0.005f;
		gl_Position.x += cos(u_DeltaTime * 100) * strength;
		gl_Position.y += cos(u_DeltaTime * 150) * strength;
	}

}
		)";
		fragmentCode = R"(
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
			if (xDistance + yDistance + TexCoords.x + TexCoords.y > u_TransitionCounter * 4f) {
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
			if (xDistance + yDistance + TexCoords.x + TexCoords.y > (1 - u_TransitionCounter) * 4f) {
				color = vec4(1.0, 0.0, 1.0, 1.0);
			}
		}
	}

}
		)";
#endif
		resourceManager.loadShaderFromString(vertexCode, fragmentCode, "postprocessing");
		printf("Compiled PostProcessing Shader \n");

		// grid shader
#ifdef CROCODILE_EMSCRIPTEN
		vertexCode = R"(#version 300 es

precision highp float;

in vec2 aPos;
in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 ScreenCoords;
out float ScreenWidth;
out float ScreenHeight;

uniform float u_ScreenWidth;
uniform float u_ScreenHeight;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f); 
	TexCoords = aTexCoords;
	vec3 ndc = gl_Position.xyz / gl_Position.w;
	vec2 viewportCoords = ndc.xy * 0.5 + 0.5;
	ScreenCoords = viewportCoords * vec2(viewportCoords * vec2(u_ScreenWidth, u_ScreenHeight));
	ScreenWidth = u_ScreenWidth;
	ScreenHeight = u_ScreenHeight;
}
		)";

		fragmentCode = R"(#version 300 es

precision mediump float;

in vec2 TexCoords;
in vec2 ScreenCoords;
in float ScreenWidth;
in float ScreenHeight;

out vec4  color;

uniform float u_OffsetX;
uniform float u_OffsetY;
uniform float u_GridSize;
uniform float u_Zoom;
uniform float u_Alpha;

void main()
{
	// base colour
	color = vec4(0.0, .5, .5, 1.0);

	int gridX = int((ScreenCoords.x + u_OffsetX) / (u_GridSize * u_Zoom));
	int gridY = int((ScreenCoords.y - u_OffsetY) / (u_GridSize * u_Zoom));

	//if (mod(mod(gridX, 2) + mod(gridY, 2), 2) == 0)
	//	color = vec4(1.0, .5, .5, 1.0);

	// apply alpha
	color.a = u_Alpha;
}
		)";

#else
		vertexCode = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 ScreenCoords;
out float ScreenWidth;
out float ScreenHeight;

uniform float u_ScreenWidth;
uniform float u_ScreenHeight;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f); 
	TexCoords = aTexCoords;
	vec3 ndc = gl_Position.xyz / gl_Position.w;
	vec2 viewportCoords = ndc.xy * 0.5 + 0.5;
	ScreenCoords = viewportCoords * vec2(viewportCoords * vec2(u_ScreenWidth, u_ScreenHeight));
	ScreenWidth = u_ScreenWidth;
	ScreenHeight = u_ScreenHeight;
}
		)";
		fragmentCode = R"(
#version 330 core
in vec2 TexCoords;
in vec2 ScreenCoords;
in float ScreenWidth;
in float ScreenHeight;

out vec4  color;

uniform float u_OffsetX;
uniform float u_OffsetY;
uniform float u_GridSize;
uniform float u_Zoom;
uniform float u_Alpha;

void main()
{
	// base colour
	color = vec4(0.0, .5, .5, 1.0);

	int gridX = int((ScreenCoords.x + u_OffsetX) / (u_GridSize * u_Zoom));
	int gridY = int((ScreenCoords.y - u_OffsetY) / (u_GridSize * u_Zoom));

	if (mod(mod(gridX, 2) + mod(gridY, 2), 2) == 0)
		color = vec4(1.0, .5, .5, 1.0);

	// apply alpha
	color.a = u_Alpha;
}
		)";
#endif
		resourceManager.loadShaderFromString(vertexCode, fragmentCode, "grid");
		printf("Compiled Grid Shader \n");
	}

	void Application::loadTextures()
	{
		resourceManager.loadTexture("assets/textures/waterDUDV.png", "waterDUDV", true);
	}

	void Application::loadAudio()
	{
	}

}