#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out float Height;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// water rendering
uniform vec4 u_ClippingPlane;

void main()
{
	// Output for fragment shader
	TexCoords = aTexCoords;
	FragPos = vec3(u_Model * vec4(aPos, 1.0));
	Normal = aNormal;
	Height = aPos.y;

	gl_ClipDistance[0] = dot(vec4(aPos, 1.0), u_ClippingPlane);
	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
}
