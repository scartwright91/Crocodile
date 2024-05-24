#version 330 core
out vec4 colour;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in float Height;

uniform float u_AmbientLighting;
uniform vec3 u_LightPosition;
uniform vec3 u_LightColour;
uniform vec3 u_CameraPosition;

uniform sampler2D u_EarthTexture;
uniform float u_MaxHeight;
uniform float u_MinHeight;


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

    // colour mixing based on altitude / depth
    float f1 = abs(Height / u_MaxHeight);
    float f2 = abs(Height / u_MinHeight);
    
    if (Height > 0)
        col = light * texture2D(u_EarthTexture, TexCoords).rgb;
    else
        col = light * mix(vec3(0.02, 0.88, 0.81), vec3(0.11, 0.0, 0.57), f2);

    colour = vec4(col, 1.0);

}