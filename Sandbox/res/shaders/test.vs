#version 330 core

layout(location = 0) in vec3 a_position;

void main()
{
    vec3 v = a_position;
    gl_Position = vec4(a_position, 1.0);
}
