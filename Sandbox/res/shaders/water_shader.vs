#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec4 ClipSpace;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

const int tiling = 100;

void main()
{
	vec4 worldPosition = u_Model * vec4(aPos, 1.0f);
	FragPos = worldPosition.xyz;
	TexCoords = aTexCoords * tiling;
	ClipSpace = u_Projection * u_View * worldPosition;
	gl_Position = ClipSpace;
}
