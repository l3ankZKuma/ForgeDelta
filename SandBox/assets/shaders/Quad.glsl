#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location =3 ) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

out vec3 v_Color;
out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core

in vec3 v_Color;
in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform vec4 u_Color;
uniform float u_TilingFactor;
uniform vec4 u_TintColor;

void main()
{
    if (u_UseTexture) {
        FragColor = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_TintColor;
    } else {
      FragColor = u_Color;
    }
}
