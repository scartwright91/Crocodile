#version 330 core

uniform vec3 u_SurfaceColour;
uniform float u_Alpha;

void main()
{
    // colour = vec4(u_SurfaceColour, u_Alpha);
    gl_FragColor = vec4(u_SurfaceColour, u_Alpha);
}