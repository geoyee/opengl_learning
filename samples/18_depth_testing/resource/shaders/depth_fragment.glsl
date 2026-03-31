#version 330 core

layout(location = 0) out vec4 aColor;

in vec2 v_TexCoords;

uniform float u_Near;
uniform float u_Far;

float LinearizeDepth(float depth);

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / u_Far;
    aColor = vec4(vec3(depth), 1.0);
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));
}