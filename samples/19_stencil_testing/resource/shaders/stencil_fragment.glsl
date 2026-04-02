#version 330 core

layout(location = 0) out vec4 aColor;

in vec2 v_TexCoords;

void main()
{
    aColor = vec4(0.04, 0.28, 0.26, 1.0);
}