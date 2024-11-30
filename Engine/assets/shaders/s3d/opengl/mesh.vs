#version 330 core

// Input vertex data from the vertex buffer (provided by OpenGL)
layout(location = 0) in vec3 aPosition;  // Vertex position
layout(location = 1) in vec3 aNormal;    // Vertex normal
layout(location = 2) in vec2 aTexCoords;  // Vertex texture coordinates
// Output data to the fragment shader
out vec3 FragPos;    // Position of the fragment in world space
out vec3 Normal;     // Normal vector for the fragment
out vec2 TexCoords;  // Texture coordinates for the fragment
// Uniforms for transformation and lighting
uniform mat4 model;      // Model matrix
uniform mat4 view;       // View matrix
uniform mat4 projection; // Projection matrix

void main()
{
    // Calculate the vertex position in world space
    FragPos = vec3(model * vec4(aPosition, 1.0));
    // Transform the normal vector to world space (and normalize)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    // Apply model, view, and projection transformations to get clip space position
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
