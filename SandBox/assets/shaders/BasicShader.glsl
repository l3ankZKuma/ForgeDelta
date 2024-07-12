#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location =1) in vec3 a_Color;

uniform mat4 model;
uniform mat4 vp;

out vec3 ourColor;
void main()
{
  gl_Position = vp * model*vec4(a_Position, 1.0);
  ourColor = a_Color;
}


#type fragment
#version 410 core

out vec4 FragColor;
in vec3 ourColor;

void main()
{
  FragColor = vec4(ourColor, 1.0); // Output red color
}
