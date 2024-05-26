#version 330 core
out vec4 colour;

in vec4 ClipSpace;

uniform sampler2D u_Reflection;
uniform sampler2D u_Refraction;

void main()
{
    vec2 ndc = (ClipSpace.xy / ClipSpace.w) / 2.0 + 0.5;

    vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y);
    vec2 refractionTexCoords = vec2(ndc.x, ndc.y);

    vec4 reflection = texture2D(u_Reflection, reflectionTexCoords);
    vec4 refraction = texture2D(u_Refraction, refractionTexCoords);

    colour = mix(reflection, refraction, 0.3);
}