#type vertex
#version 420

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
uniform mat4 vp;
uniform mat4 model;
out vec2 ourTexCoord;

void main() {
  gl_Position = vp * model * vec4(aPos, 1.0);
  ourTexCoord = aTexCoord;
}

#type fragment
#version 420 core

out vec4 fragColor;
in vec2 ourTexCoord;
uniform sampler2D texture1;

void main() {
  fragColor = texture(texture1, ourTexCoord);
}
