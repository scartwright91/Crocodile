#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;
uniform float u_Alpha;

void main()
{    
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Text, TexCoords).r);
	color = vec4(u_TextColor, u_Alpha) * sampled;
}
