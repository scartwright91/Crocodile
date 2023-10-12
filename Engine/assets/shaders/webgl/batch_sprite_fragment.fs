#version 300 es

precision mediump float;

in vec2 TexCoords;
out vec4 color;

uniform float u_Alpha;
uniform sampler2D u_Texture;

void main() {
    color = texture(u_Texture, vec2(TexCoords.x, TexCoords.y)) * vec4(1, 1, 1, u_Alpha);
}
