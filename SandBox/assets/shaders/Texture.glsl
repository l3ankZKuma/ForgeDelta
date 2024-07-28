#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location= 1) in vec4 a_Color;
layout(location =2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;
out vec2 ourTexCoord;
out vec4 ourColor;
out float ourTextureIndex;
out float ourTilingFactor;



void main()
{

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
	ourTexCoord = a_TexCoord;
	ourColor = a_Color;
	ourTextureIndex = a_TextureIndex;
  ourTilingFactor = a_TilingFactor;


}

#type fragment
#version 410 core


in vec4 ourColor;
in vec2 ourTexCoord;
in float ourTextureIndex;
in float ourTilingFactor;

uniform sampler2D u_Textures[32];

out vec4 FragColor;

void main()
{
	vec4 texColor = texture(u_Textures[int(ourTextureIndex)], ourTexCoord * ourTilingFactor);
	FragColor = texColor * ourColor;
}
