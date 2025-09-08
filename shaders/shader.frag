#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

// Propriedades do Material (do objeto)
uniform vec3 objectColor;
uniform vec3 viewPos; // Posição da câmera

// Luz Direcional (Sol/Luz do Teto)
struct DirLight {
    vec3 direction;
    vec3 color;
};
uniform DirLight dirLight;

// Luz do Baú (Luz Pontual)
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity; // Usado para ligar/desligar

    // Atenuação
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight chestLight;

// Funções para calcular cada tipo de luz
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Começa com a contribuição da luz direcional global
    vec3 lighting = CalcDirLight(dirLight, norm, viewDir);

    // Adiciona a contribuição da luz do baú (se estiver acesa)
    lighting += CalcPointLight(chestLight, norm, FragPos, viewDir);

    // A cor final é a cor do objeto multiplicada pela iluminação total
    FragColor = vec4(lighting * objectColor, 1.0);
}

// Função que calcula a luz direcional
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Ambiente (para a luz global)
    float ambientStrength = 0.3; // Luz ambiente um pouco mais forte
    vec3 ambient = ambientStrength * light.color;

    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff;

    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.5) * spec * light.color; // Brilho

    return (ambient + diffuse + specular);
}

// Função que calcula a luz pontual
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if (light.intensity <= 0.0) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(light.position - fragPos);

    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff;

    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.5) * spec * light.color;

    // Atenuação
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    diffuse  *= attenuation;
    specular *= attenuation;

    return (diffuse + specular) * light.intensity;
}
