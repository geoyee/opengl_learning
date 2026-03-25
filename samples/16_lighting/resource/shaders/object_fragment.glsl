#version 330 core

layout(location = 0) out vec4 aColor;

in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform float u_Shininess;

void main()
{
    // Ambient Lighting
    vec3 ambient = u_AmbientStrength * u_LightColor;

    // Diffuse Lighting
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Specular Lighting
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
    vec3 specular = u_SpecularStrength * spec * u_LightColor;

    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
    aColor = vec4(result, 1.0);
}