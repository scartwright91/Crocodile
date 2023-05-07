#version 300 es

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
