#version 330 core

layout(location = 0) out vec4 aColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in vec2 v_TexCoords;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform DirectionalLight u_DLight;
uniform PointLight u_PLight;
uniform SpotLight u_SLight;

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir, vec3 diffMat, vec3 specMat);
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 diffMat, vec3 specMat);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 diffMat, vec3 specMat);

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 diffMat = vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specMat = vec3(texture(u_Material.specular, v_TexCoords));

    vec3 result = CalcDirLight(u_DLight, norm, viewDir, diffMat, specMat);
    result += CalcPointLight(u_PLight, norm, v_FragPos, viewDir, diffMat, specMat);
    result += CalcSpotLight(u_SLight, norm, v_FragPos, viewDir, diffMat, specMat);
    aColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir, vec3 diffMat, vec3 specMat)
{
    // Ambient Lighting
    vec3 ambient = u_DLight.ambient * diffMat;
    // Diffuse Lighting
    vec3 lightDir = normalize(-u_DLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_DLight.diffuse * diff * diffMat;
    // Specular Lighting
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_DLight.specular * spec * specMat;
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 diffMat, vec3 specMat)
{
    // Ambient Lighting
    vec3 ambient = u_PLight.ambient * diffMat;
    // Diffuse Lighting
    vec3 lightDir = normalize(u_PLight.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_PLight.diffuse * diff * diffMat;
    // Specular Lighting
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_PLight.specular * spec * specMat;
    // Attenuation
    float distance = length(u_PLight.position - v_FragPos);
    float attenuation =
        1.0 / (u_PLight.constant + u_PLight.linear * distance + u_PLight.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 diffMat, vec3 specMat)
{
    // Ambient Lighting
    vec3 ambient = u_SLight.ambient * diffMat;
    // Diffuse Lighting
    vec3 lightDir = normalize(u_SLight.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_SLight.diffuse * diff * diffMat;
    // Specular Lighting
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_SLight.specular * spec * specMat;
    // Spotlight (Soft edges)
    float theta = dot(lightDir, normalize(-u_SLight.direction));
    float epsilon = (u_SLight.cutOff - u_SLight.outerCutOff);
    float intensity = clamp((theta - u_SLight.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
    // Attenuation
    float distance = length(u_SLight.position - v_FragPos);
    float attenuation =
        1.0 / (u_SLight.constant + u_SLight.linear * distance + u_SLight.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}