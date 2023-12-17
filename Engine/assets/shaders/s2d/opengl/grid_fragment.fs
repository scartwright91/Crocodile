#version 330 core
in vec2 TexCoords;
in vec2 ScreenCoords;
in float ScreenWidth;
in float ScreenHeight;

out vec4  color;

uniform float u_OffsetX;
uniform float u_OffsetY;
uniform float u_GridSize;
uniform float u_Zoom;
uniform vec3  u_Color1;
uniform vec3  u_Color2;
uniform float u_Alpha;

void main()
{
	// base colour
	color = vec4(u_Color1, 1.0);

	int gridX = int((ScreenCoords.x + u_OffsetX) / (u_GridSize * u_Zoom));
	int gridY = int((ScreenCoords.y - u_OffsetY) / (u_GridSize * u_Zoom));

	if (mod(mod(gridX, 2) + mod(gridY, 2), 2) == 0)
		color = vec4(u_Color2, 1.0);

	// apply alpha
	color.a = u_Alpha;
}
