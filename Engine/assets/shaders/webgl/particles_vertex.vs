#version 300 es

precision highp float;

in vec4 vertex;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec2 u_Offset;
uniform float u_Scale;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4((vertex.xy * u_Scale) + u_Offset, 0.0, 1.0);
}