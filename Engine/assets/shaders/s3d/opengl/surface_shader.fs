#version 330 core
out vec4 colour;

in vec3 FragPos;
in vec3 Normal;
in float Height;

uniform float u_AmbientLighting;
uniform vec3 u_LightPosition;
uniform vec3 u_LightColour;
uniform vec3 u_CameraPosition;
uniform vec3 u_SurfaceColour;

void main()
{
    // ambient
    vec3 ambient = u_AmbientLighting * u_LightColour;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColour;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_CameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColour;

    vec3 light = (ambient + diffuse + specular);
    if (Height > 0)
        colour = vec4(light * u_SurfaceColour, 1.0);
    else
        colour = vec4(0.03, 0.28, 0.51, 1.0);

}