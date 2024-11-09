#version 330 core

// Input data from the vertex shader
in vec3 FragPos;    // Position of the fragment in world space
in vec3 Normal;     // Normal vector of the fragment

// Output color
out vec4 FragColor;

// Uniforms for lighting and material properties
uniform vec3 lightPos;      // Position of the light source
uniform vec3 viewPos;       // Position of the camera/viewer
uniform vec3 lightColor;    // Color of the light
uniform vec3 objectColor;   // Color of the object

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}