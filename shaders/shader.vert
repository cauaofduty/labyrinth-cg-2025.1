#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Gerar coordenadas de textura baseadas na posição
    TexCoords = aPos.xy * 1.0; // Usar xy com escala 1.0 para melhor visibilidade
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
