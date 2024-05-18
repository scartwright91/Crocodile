#version 330 core
out vec4 colour;

in float Height;

uniform float u_AmbientLighting;
uniform vec3 u_LightPosition;
uniform vec3 u_LightColour;

void main()
{
    // ambient

    // diffuse

    // specular

    float h = Height;
    colour = vec4(h, h, h, 1.0);
}