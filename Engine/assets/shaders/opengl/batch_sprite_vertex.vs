#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

// vertix position calculation
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    TexCoords = vertex.zw;
    gl_Position = u_Projection * u_View * u_Model * vec4(vertex.xy, 0.0, 1.0);
}