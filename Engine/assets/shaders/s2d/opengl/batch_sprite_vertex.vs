#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec3 FragPos;

// vertex position calculation
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    TexCoords = vertex.zw;
	FragPos = vec3(u_Model * vec4(vertex.xy, 0.0, 1.0));
    gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
}