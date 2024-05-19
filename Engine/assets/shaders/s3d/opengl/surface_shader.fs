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
    
    vec3 col;
    if (Height > 0)
        col = vec3(1.f, 1.f, 0.f);
    else
        col = vec3(0., 0.5, 0.5);

    colour = vec4(light * col, 1.0);

}