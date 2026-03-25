#version 330 core

layout(location = 0) out vec4 aColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

void main()
{
    // Ambient Lighting
    vec3 ambient = u_Light.ambient * u_Material.ambient;

    // Diffuse Lighting
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

    // Specular Lighting
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * u_Material.specular);

    vec3 result = ambient + diffuse + specular;
    aColor = vec4(result, 1.0);
}