#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include "Shader.h"
#include "TextRenderer.h"

// Estruturas de Dados da Cena
struct SceneObject {
    GLuint vao = 0;
    int vertexCount = 0;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 boundingBoxMin, boundingBoxMax;
};

struct Chest {
    SceneObject* base = nullptr;
    SceneObject* lid = nullptr;
    std::string base_name;
    bool hasBeenCounted = false;
    bool isAnimating = false;
    bool isOpen = false;
    float lidOffsetY = 0.0f;
    float targetLidOffsetY = 0.0f;
    float animationSpeed = 2.0f;
    glm::vec3 lightColor = glm::vec3(1.0f, 0.85f, 0.4f);
    float currentLightIntensity = 0.0f;
    float targetLightIntensity = 0.0f;
    float lightFadeSpeed = 3.0f;

    bool toggleOpen();
    void update(float dt);
    glm::vec3 getLightWorldPosition() const;
};

// Definição da Classe Principal do Jogo
class Game
{
public:
    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    bool IsRunning;

    // Funções de Callback para o GLFW
    void FramebufferSizeCallback(int width, int height);
    void MouseCallback(double xpos, double ypos);
    void MouseButtonCallback(int button, int action, int mods);

private:
    // Variáveis da Janela e Renderização
    unsigned int Width, Height;
    GLFWwindow* Window;
    Shader* SceneShader = nullptr;
    TextRenderer* Text = nullptr;

    // Estado do Jogo
    int chestsOpenedCount = 0;
    const int CHESTS_TO_WIN = 3;
    bool gameWon = false;
    bool portalIsActive = false;
    std::string uiMessage;
    float uiMessageTimer = 0.0f;

    // Câmera
    glm::vec3 cameraPos   = glm::vec3(9.0f, 1.5f, 20.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    float yaw   = -90.0f;
    float pitch =  0.0f;
    float lastX, lastY;
    bool firstMouse = true;

    // Objetos da Cena
    std::map<std::string, SceneObject> sceneObjects;
    std::vector<SceneObject*> colliders;
    SceneObject* portalObject = nullptr;
    std::vector<std::unique_ptr<Chest>> chests;

    // Funções privadas da classe Game
    void loadScene(const std::string& path);
    bool checkWallCollision(glm::vec3 futurePos);
    void handleInteraction();
};

// Funções "Wrapper" para que o GLFW, que é uma biblioteca em C, possa chamar os métodos da nossa classe C++
void framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height);
void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback_wrapper(GLFWwindow* window, int button, int action, int mods);
