#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    v_FragPos = vec3(u_Model * aPos);
    v_Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    gl_Position = u_Proj * u_View * u_Model * aPos;
}