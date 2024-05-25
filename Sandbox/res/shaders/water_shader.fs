#version 330 core
out vec4 colour;

uniform vec3 u_SurfaceColour;
uniform float u_Alpha;

void main()
{
    colour = vec4(u_SurfaceColour, 1.0);
    colour.a = u_Alpha;
}