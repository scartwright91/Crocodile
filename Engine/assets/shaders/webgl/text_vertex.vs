#version 300 es

precision highp float;

in vec4 vertex;

out vec2 TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_View * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}