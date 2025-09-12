#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// Propriedades do Material (do objeto)
uniform vec3 objectColor;
uniform vec3 viewPos; // Posição da câmera

// Texturas
uniform sampler2D diffuseTexture;
uniform int useTexture;

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
vec3 CalcDirLightPBR(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metallic);
vec3 CalcPointLightPBR(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float roughness, float metallic);

// Funções PBR
vec3 fresnelSchlick(float cosTheta, vec3 F0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

// Função para calcular normal do mapa normal
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalTexture, TexCoords).xyz * 2.0 - 1.0;
    
    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);
    
    vec3 N   = normalize(Normal);
    vec3 T   = normalize(Tangent);
    vec3 B   = normalize(Bitangent);
    mat3 TBN = mat3(T, B, N);
    
    return normalize(TBN * tangentNormal);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Começa com a contribuição da luz direcional global
    vec3 lighting = CalcDirLight(dirLight, norm, viewDir);

    // Adiciona a contribuição da luz do baú (se estiver acesa)
    lighting += CalcPointLight(chestLight, norm, FragPos, viewDir);

    // A cor final é a cor do objeto multiplicada pela iluminação total
    vec3 finalColor = objectColor;
    if (useTexture == 1) {
        // Mostrar textura diretamente sem iluminação
        vec4 texColor = texture(diffuseTexture, TexCoords);
        finalColor = texColor.rgb;
    } else {
        finalColor = lighting * finalColor;
    }
    FragColor = vec4(finalColor, 1.0);
}

// Funções PBR
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265359 * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Função que calcula a luz direcional PBR
vec3 CalcDirLightPBR(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metallic)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfDir = normalize(lightDir + viewDir);
    
    // F0 para reflexão de Fresnel
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    
    // Ambiente
    vec3 ambient = 0.03 * albedo * light.color;
    
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, halfDir, roughness);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);
    vec3 F = fresnelSchlick(max(dot(halfDir, viewDir), 0.0), F0);
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * NdotL + 0.0001;
    vec3 specular = numerator / denominator;
    
    return ambient + (kD * albedo / 3.14159265359 + specular) * light.color * NdotL;
}

// Função que calcula a luz pontual PBR
vec3 CalcPointLightPBR(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float roughness, float metallic)
{
    if (light.intensity <= 0.0) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    
    // F0 para reflexão de Fresnel
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, halfDir, roughness);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);
    vec3 F = fresnelSchlick(max(dot(halfDir, viewDir), 0.0), F0);
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * NdotL + 0.0001;
    vec3 specular = numerator / denominator;
    
    // Atenuação
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 radiance = light.color * attenuation;
    
    return (kD * albedo / 3.14159265359 + specular) * radiance * NdotL * light.intensity;
}

// Função que calcula a luz direcional (versão antiga para compatibilidade)
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

// Função que calcula a luz pontual (versão antiga para compatibilidade)
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
