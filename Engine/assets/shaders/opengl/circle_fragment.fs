#version 330 core

uniform float u_Radius;
uniform vec3 u_Color;
uniform float u_Alpha;
uniform float u_LayerAlpha;

in vec2 localPosition;
out vec4 color;

void main()
{

    // Parameters
    vec3 circleColor = vec3(0.85, 0.35, 0.2);
    float thickness = 0.5;
    float fade = 0.005;

    // -1 -> 1 local space, adjusted for aspect ratio
    vec2 uv = localPosition;
    
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(uv);
    vec3 col = vec3(smoothstep(0.0, fade, distance));
    col *= vec3(smoothstep(thickness + fade, thickness, distance));

    // Set output color
    color = vec4(col, 1.0);
    color.rgb *= u_Color;
	// color.a = u_Alpha;

	// color = vec4(u_Color, u_Alpha) * vec4(1, 1, 1, u_LayerAlpha);
}
