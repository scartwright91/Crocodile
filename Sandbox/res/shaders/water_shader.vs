#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec4 ClipSpace;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	ClipSpace = u_Projection * u_View * u_Model * vec4(aPos, 1.0f);
	gl_Position = ClipSpace;
}
