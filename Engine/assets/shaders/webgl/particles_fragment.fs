#version 300 es

precision mediump float;

out vec4 color;

uniform vec3 u_Color;
uniform float u_Alpha;

void main()
{
	color = vec4(u_Color, u_Alpha);
}
