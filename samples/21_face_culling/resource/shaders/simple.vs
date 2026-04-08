#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoords;

out vec2 v_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    v_TexCoords = texCoords;
    gl_Position = u_Proj * u_View * u_Model * vec4(pos, 1.0);
}