#version 300 es

precision mediump float;

in vec2 vertex;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(vertex.xy, 0.0, 1.0);
}
