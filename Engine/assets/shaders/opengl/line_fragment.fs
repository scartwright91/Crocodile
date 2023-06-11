#version 330 core

uniform vec3 u_Color;
uniform float u_Alpha;

out vec4 color;

void main()
{
	color = vec4(u_Color, u_Alpha);
}
