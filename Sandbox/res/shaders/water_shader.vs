#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 ClipSpace;
out vec3 ViewDirection;
out vec3 LightDirection;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_CameraPosition;
uniform vec3 u_LightPosition;

const int tiling = 100;

void main()
{
	vec4 worldPosition = u_Model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords * tiling;
	ClipSpace = u_Projection * u_View * worldPosition;
	ViewDirection = u_CameraPosition - worldPosition.xyz;
	// LightDirection = worldPosition.xyz - u_LightPosition;
	LightDirection = u_LightPosition - worldPosition.xyz;
	gl_Position = ClipSpace;
}
