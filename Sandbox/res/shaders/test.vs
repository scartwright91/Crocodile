#version 330 core

// Input vertex attributes (position, normal)
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// Output to the fragment shader
out vec3 FragPos;
out vec3 Normal;

// Uniforms for transformation matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    // Calculate the vertex position in world space
    FragPos = vec3(u_Model * vec4(aPosition, 1.0));
    
    // Transform the normal to world space and normalize it
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    
    // Transform the vertex position to clip space
    gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
}
