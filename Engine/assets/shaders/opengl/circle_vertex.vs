#version 330 core

layout (location = 0) in vec2 vertex;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 localPosition;

void main()
{
	localPosition = vertex.xy * 2 - 1;
	gl_Position = u_Projection * u_View * u_Model * vec4(vertex.xy, 0.0, 1.0);
}
