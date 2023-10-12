#version 300 es

precision highp float;

in vec2 aPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 localPosition;

void main()
{
	localPosition = aPos * vec2(2) - vec2(1);
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 0.0, 1.0);
}
