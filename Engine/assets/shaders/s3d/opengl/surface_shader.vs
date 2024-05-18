#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out float Height;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	// Output for fragment shader
	FragPos = vec3(u_Model * vec4(aPos, 1.0));
	Normal = aNormal;
	Height = aPos.y;

	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
}
