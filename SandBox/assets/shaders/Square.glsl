#type vertex
#version 420

layout(location = 0) in vec3 aPos;
uniform mat4 vp;
uniform mat4 model;
void main() {
        gl_Position = vp * model * vec4(aPos, 1.0);
}


#type fragment
#version 420 core

uniform vec4 color;
out vec4 fragColor;
void main() {

  fragColor = color;
}
