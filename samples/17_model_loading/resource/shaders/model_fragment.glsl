#version 330 core

layout(location = 0) out vec4 aColor;

in vec2 v_TexCoords;

uniform sampler2D u_TextureDiffuse_0;

void main()
{
    aColor = texture(u_TextureDiffuse_0, v_TexCoords);
}