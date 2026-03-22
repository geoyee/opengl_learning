#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Textures[2];

void main()
{
    vec4 texColor0 = texture(u_Textures[0], v_TexCoord);
    vec4 texColor1 = texture(u_Textures[1], v_TexCoord);
    color = texColor1 * (1.0 - texColor0.a) + texColor0 * texColor0.a;
}