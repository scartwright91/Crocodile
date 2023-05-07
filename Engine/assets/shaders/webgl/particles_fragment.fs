#version 300 es

precision mediump float;

in vec4 particleColor;
out vec4 color;

void main()
{
	color = particleColor;
}
