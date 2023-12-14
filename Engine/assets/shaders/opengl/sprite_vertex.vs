#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;
out vec3 FragPos;

// vertix position calculation
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// texture coordination calculation
uniform float u_NumberOfRows;
uniform float u_NumberOfCols;
uniform vec2 u_TextureOffset;
uniform bool u_FlipX;
uniform bool u_FlipY;

// squeeze effect
uniform bool u_Squeeze;
uniform vec2 u_Deformation;

void main()
{

	if (u_NumberOfRows == 1.0 && u_NumberOfCols == 1.0)
		TexCoords = vertex.zw;
	else
		TexCoords = vec2(vertex.z / u_NumberOfCols + u_TextureOffset.x, vertex.w / u_NumberOfRows + u_TextureOffset.y);

	if (u_FlipX)
		TexCoords.x = 1 - TexCoords.x;
	if (u_FlipY)
		TexCoords.y = 1 - TexCoords.y;

	vec2 v = vertex.xy;
	if (u_Squeeze)
	{
		v -= vec2(0.5);
		v *= vec2(1. + u_Deformation.x, 1. + u_Deformation.y);
		v += vec2(0.5);
	}

	FragPos = vec3(u_Model * vec4(v, 0.0, 1.0));
	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);

}
