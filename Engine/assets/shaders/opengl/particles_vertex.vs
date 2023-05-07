#version 330 core

layout (location = 0) in vec4 vertex;

out vec4 particleColor;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec2 u_Offset;
uniform float u_Scale;

uniform vec3 u_Color;
uniform float u_Alpha;

void main()
{
	particleColor = vec4(u_Color, u_Alpha);
	gl_Position = u_Projection * u_View * u_Model * vec4((vertex.xy * u_Scale) + u_Offset, 0.0, 1.0);
}
