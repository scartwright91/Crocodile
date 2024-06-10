#version 330 core
out vec4 color;

in vec2 TexCoord;

uniform vec3 u_Colour;

void main()
{
	color = vec4(u_Colour, 1.f);
}