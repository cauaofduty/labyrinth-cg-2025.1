#pragma once

// ============================================================================
// BIBLIOTECAS NECESSÁRIAS
// ============================================================================
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include "Shader.h"
#include "TextRenderer.h"

// ============================================================================
// ESTRUTURAS DE DADOS DA CENA
// ============================================================================

// Representa um objeto 3D na cena
struct SceneObject {
    GLuint vao = 0;                    // Vertex Array Object
    int vertexCount = 0;               // Número de vértices
    glm::mat4 modelMatrix = glm::mat4(1.0f);  // Matriz de transformação
    glm::vec3 boundingBoxMin, boundingBoxMax; // Bounding box para colisão
};

// Representa um baú com animação e iluminação
struct Chest {
    SceneObject* base = nullptr;       // Parte inferior do baú
    SceneObject* lid = nullptr;        // Tampa do baú
    std::string base_name;             // Nome do objeto base
    bool hasBeenCounted = false;       // Se já foi contado para vitória
    bool isAnimating = false;          // Se está em animação
    bool isOpen = false;               // Estado de abertura
    float lidOffsetY = 0.0f;           // Offset atual da tampa
    float targetLidOffsetY = 0.0f;     // Offset alvo da tampa
    float animationSpeed = 2.0f;       // Velocidade da animação
    glm::vec3 lightColor = glm::vec3(1.0f, 0.9f, 0.6f); // Cor dourada da luz
    float currentLightIntensity = 0.0f; // Intensidade atual da luz
    float targetLightIntensity = 0.0f;  // Intensidade alvo da luz
    float lightFadeSpeed = 3.0f;       // Velocidade do fade da luz

    bool toggleOpen();                 // Alternar estado de abertura
    void update(float dt);             // Atualizar animação
    glm::vec3 getLightWorldPosition() const; // Posição da luz no mundo
};

// ============================================================================
// CLASSE PRINCIPAL DO JOGO
// ============================================================================
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
