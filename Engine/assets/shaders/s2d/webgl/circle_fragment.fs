#version 300 es

precision mediump float;

uniform float u_Thickness;
uniform float u_Fade;
uniform vec3 u_Color;
uniform float u_Alpha;
uniform float u_LayerAlpha;

in vec2 localPosition;
out vec4 color;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(localPosition);
    vec3 col = vec3(smoothstep(0.0, u_Fade, distance));
    col *= vec3(smoothstep(u_Thickness + u_Fade, u_Thickness, distance));

    // Set output color
    color = vec4(col, col.r * 1.0);
    color *= (vec4(u_Color, u_Alpha) * vec4(1, 1, 1, u_LayerAlpha));
}
