#version 330 core
out vec4 colour;

in vec2 TexCoords;
in vec4 ClipSpace;
in vec3 ToCameraPositon;

uniform sampler2D u_Reflection;
uniform sampler2D u_Refraction;
uniform sampler2D u_DuDv;
uniform float u_Time;

const float distortionStrength = 0.05;

void main()
{
    vec2 ndc = (ClipSpace.xy / ClipSpace.w) / 2.0 + 0.5;
    vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y);
    vec2 refractionTexCoords = vec2(ndc.x, ndc.y);

    // calculate surface distortion
    vec2 distortion1 = distortionStrength * (
        texture2D(
            u_DuDv,
            vec2(TexCoords.x + u_Time, TexCoords.y)
        ).rg * 2.0 - 1.0
    );
    vec2 distortion2 = distortionStrength * (
        texture2D(
            u_DuDv,
            vec2(-TexCoords.x + u_Time, TexCoords.y + u_Time)
        ).rg * 2.0 - 1.0
    );
    vec2 distortion = distortion1 + distortion2;
    
    // apply distortion to texture coordiantes
    reflectionTexCoords += distortion;
    reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999);
    reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

    refractionTexCoords += distortion;
    refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);

    // sample from reflection and refraction textures
    vec4 reflection = texture2D(u_Reflection, reflectionTexCoords);
    vec4 refraction = texture2D(u_Refraction, refractionTexCoords);

    // calculate fresnel effect based on camera position
    vec3 viewFactor = normalize(ToCameraPositon);
    float fresnel = dot(viewFactor, vec3(0, 1, 0));

    colour = mix(reflection, refraction, 0.5);

}