#version 300 es

precision highp float;

in vec2 aPos;
in vec2 aTexCoords;

out vec2 TexCoords;

// vertix position calculation
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    TexCoords = TexCoords;
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 0.0, 1.0);
}