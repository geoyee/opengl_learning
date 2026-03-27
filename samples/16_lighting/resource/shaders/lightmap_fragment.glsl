#version 330 core

layout(location = 0) out vec4 aColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 v_TexCoords;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

void main()
{
    vec3 diffTmp = vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specTmp = vec3(texture(u_Material.specular, v_TexCoords));

    // Ambient Lighting
    vec3 ambient = u_Light.ambient * diffTmp;

    // Diffuse Lighting
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * diffTmp;

    // Specular Lighting
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * specTmp;

    vec3 result = ambient + diffuse + specular;
    aColor = vec4(result, 1.0);
}