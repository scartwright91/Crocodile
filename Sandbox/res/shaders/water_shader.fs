#version 330 core
out vec4 colour;

in vec2 TexCoords;
in vec4 ClipSpace;
in vec3 ViewDirection;
in vec3 LightDirection;

uniform sampler2D u_Reflection;
uniform sampler2D u_Refraction;
uniform sampler2D u_DuDv;
uniform sampler2D u_NormalMap;
uniform float u_Time;
uniform vec3 u_LightColour;

const float distortionStrength = 0.05;
const float shineDamper = 20.0;
const float reflectivity = 0.6;

void main()
{
    vec2 ndc = (ClipSpace.xy / ClipSpace.w) / 2.0 + 0.5;
    vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y);
    vec2 refractionTexCoords = vec2(ndc.x, ndc.y);

    // distortion effect
    vec2 distortionTexCoords = texture2D(u_DuDv, vec2(TexCoords.x + u_Time, TexCoords.y)).rg * 0.1;
    distortionTexCoords = TexCoords + vec2(distortionTexCoords.x, distortionTexCoords.y + u_Time);
    vec2 distortion = distortionStrength * (texture2D(u_DuDv, distortionTexCoords).rg * 2.0 - 1.0);
    
    // apply distortion to texture coordiantes
    // TODO - fix distortion for reflection texture
    // reflectionTexCoords += distortion;
    reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999);
    reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

    refractionTexCoords += distortion;
    refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);

    // sample from reflection and refraction textures
    vec4 reflection = texture2D(u_Reflection, reflectionTexCoords);
    vec4 refraction = texture2D(u_Refraction, refractionTexCoords);

    // calculate fresnel effect based on camera position
    vec3 viewVector = normalize(ViewDirection);
    float fresnel = dot(viewVector, vec3(0, 1, 0));

    // normal map
    vec4 normalMapColour = texture2D(u_NormalMap, distortionTexCoords);
    vec3 normal = vec3(normalMapColour.x * 2.0 - 1.0, normalMapColour.g, normalMapColour.z * 2.0 + 1.0);    

    // lighting
    vec3 reflectedLight = reflect(normalize(LightDirection), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = u_LightColour * specular * reflectivity;

    colour = mix(reflection, refraction, fresnel);
    colour = mix(colour, vec4(0.0, 0.3, 0.7, 1.0), 0.2) + vec4(specularHighlights, 0.0);

}