#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include "Shader.h"

struct Character {
    GLuint     TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint     Advance;
};
class TextRenderer
{
public:
    TextRenderer(Shader &shader, unsigned int screenWidth, unsigned int screenHeight);
    void Load(const char* fontPath, unsigned int fontSize);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    std::map<GLchar, Character> Characters;
    Shader &TextShader;
    GLuint VAO, VBO;
};
