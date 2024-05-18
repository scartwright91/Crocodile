#version 330 core
out vec4 color;

in float Height;

void main()
{
    // float h = (Height + 16) / 32.0f;	// shift and scale the height into a grayscale value
    float h = Height;
    color = vec4(h, h, h, 1.0);
}