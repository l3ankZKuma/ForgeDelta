#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location= 1) in vec3 a_Color;
layout(location =2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);

}

#type fragment
#version 410 core


uniform vec4 u_Color;
out vec4 FragColor;

void main()
{
	FragColor = u_Color;
}