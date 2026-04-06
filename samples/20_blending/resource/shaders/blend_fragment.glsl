#version 330 core

layout(location = 0) out vec4 aColor;

in vec2 v_TexCoords;

uniform sampler2D u_Window;

void main()
{
    aColor = texture(u_Window, v_TexCoords);
}