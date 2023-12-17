#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec2 u_Offset;
uniform float u_Scale;

void main()
{
	TexCoords = vertex.zw;
	FragPos = vec3(u_Model * vec4(vertex.xy, 0.0, 1.0));
	gl_Position = u_Projection * u_View * u_Model * vec4((vertex.xy * u_Scale) + u_Offset, 0.0, 1.0);
}
