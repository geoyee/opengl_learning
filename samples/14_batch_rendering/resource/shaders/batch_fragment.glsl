#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
flat in float v_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{
    int texIndex = int(v_TexIndex);
    vec4 texColor = texture(u_Textures[texIndex], v_TexCoord);
    color = texColor;
}