#version 330 core

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Output color
out vec4 FragColor;

// Uniforms for material and light properties
uniform vec3 u_ObjectColour;
uniform vec3 u_LightColour;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;
uniform float u_AmbientLighting;

// textures
uniform sampler2D u_Texture;

void main()
{
    // Ambient lighting
    vec3 ambient = u_AmbientLighting * u_LightColour;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColour;
    
    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_CameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColour;
    
    // Combine results
    vec3 textureColor = texture(u_Texture, TexCoords).rgb;
    vec3 result = (ambient + diffuse + specular) * textureColor;
    FragColor = vec4(result, 1.0);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);

}
