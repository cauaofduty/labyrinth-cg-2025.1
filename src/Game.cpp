#include "Game.h"
#include "tiny_obj_loader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES WRAPPER
// ============================================================================
// Estas funções servem como ponte entre o GLFW (biblioteca C) e nossa classe C++
void framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) game->FramebufferSizeCallback(width, height);
}
void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) game->MouseCallback(xpos, ypos);
}
void mouse_button_callback_wrapper(GLFWwindow* window, int button, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) game->MouseButtonCallback(button, action, mods);
}

// Implementação da Classe Game
Game::Game(unsigned int width, unsigned int height) : Width(width), Height(height), IsRunning(true)
{
    if (!glfwInit()) {
        std::cout << "Falha ao inicializar GLFW" << std::endl;
        IsRunning = false;
        return;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    Window = glfwCreateWindow(Width, Height, "Labirinto Interativo", NULL, NULL);
    if (!Window) {
        std::cout << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        IsRunning = false;
        return;
    }
    glfwMakeContextCurrent(Window);

    glfwSetWindowUserPointer(Window, this);
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback_wrapper);
    glfwSetCursorPosCallback(Window, mouse_callback_wrapper);
    glfwSetMouseButtonCallback(Window, mouse_button_callback_wrapper);

    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Tentar carregar GLAD de forma mais segura
    try {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Falha ao inicializar GLAD" << std::endl;
            glfwTerminate();
            IsRunning = false;
            return;
        }
        
        // Verificar se o OpenGL foi inicializado corretamente
        const char* version = (const char*)glGetString(GL_VERSION);
        if (!version) {
            std::cout << "Falha ao obter versão do OpenGL" << std::endl;
            glfwTerminate();
            IsRunning = false;
            return;
        }
        
        std::cout << "OpenGL Version: " << version << std::endl;
    } catch (...) {
        std::cout << "Erro ao inicializar OpenGL" << std::endl;
        glfwTerminate();
        IsRunning = false;
        return;
    }

    lastX = Width / 2.0f;
    lastY = Height / 2.0f;
}

Game::~Game()
{
    delete SceneShader;
    delete Text;
    glfwTerminate();
}

// ============================================================================
// INICIALIZAÇÃO DO JOGO
// ============================================================================
void Game::Init()
{
    std::cout << "=== INICIALIZANDO JOGO ===" << std::endl;
    std::cout << "Configurando OpenGL..." << std::endl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Carregando shaders..." << std::endl;
    SceneShader = new Shader("shaders/shader.vert", "shaders/shader.frag");
    Shader* textShader = new Shader("shaders/text.vert", "shaders/text.frag");
    
    std::cout << "Inicializando TextRenderer..." << std::endl;
    Text = new TextRenderer(*textShader, Width, Height);
    Text->Load("fonts/DejaVuSansMono.ttf", 48);

    
    std::cout << "Carregando cena do labirinto..." << std::endl;
    loadScene("models/lab.obj");
    std::cout << "=== INICIALIZAÇÃO CONCLUÍDA ===" << std::endl;
}

// ============================================================================
// CARREGAMENTO DA CENA
// ============================================================================
void Game::loadScene(const std::string& path)
{
    std::cout << "Carregando arquivo: " << path << std::endl;
    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    std::cout << "Processando arquivo OBJ..." << std::endl;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        std::cout << "Erro ao carregar OBJ: " << warn << " " << err << std::endl;
        throw std::runtime_error(warn + err);
    }
    
    std::cout << "✓ Arquivo OBJ carregado com sucesso! (" << shapes.size() << " objetos encontrados)" << std::endl;
    for (const auto& shape : shapes) {
        SceneObject obj;
        std::vector<float> vertex_data;
        glm::vec3 min_bound(std::numeric_limits<float>::max());
        glm::vec3 max_bound(std::numeric_limits<float>::lowest());
        for (const auto& index : shape.mesh.indices) {
            glm::vec3 pos = { attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2] };
            vertex_data.insert(vertex_data.end(), {pos.x, pos.y, pos.z});
            min_bound = glm::min(min_bound, pos);
            max_bound = glm::max(max_bound, pos);
            if (index.normal_index >= 0 && !attrib.normals.empty()) { vertex_data.insert(vertex_data.end(), {attrib.normals[3 * index.normal_index + 0], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2]}); }
            else { vertex_data.insert(vertex_data.end(), {0.0f, 1.0f, 0.0f}); }
        }
        obj.vertexCount = shape.mesh.indices.size();
        obj.boundingBoxMin = min_bound;
        obj.boundingBoxMax = max_bound;
        GLuint VBO;
        glGenVertexArrays(1, &obj.vao);
        glGenBuffers(1, &VBO);
        glBindVertexArray(obj.vao);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
        sceneObjects[shape.name] = obj;
        std::cout << "Objeto carregado: " << shape.name << std::endl;
    }

    std::map<int, std::string> baseNames;
    std::map<int, std::string> lidNames;
    for (auto const& [name, object] : sceneObjects) {
        std::string lower_name = name;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        size_t bau_pos = lower_name.find("bau_");
        if (bau_pos != std::string::npos) {
            int chest_num = -1;
            if (sscanf(lower_name.c_str() + bau_pos, "bau_%d", &chest_num) == 1) {
                if (lower_name.find("base") != std::string::npos) baseNames[chest_num] = name;
                else if (lower_name.find("tampa") != std::string::npos) lidNames[chest_num] = name;
            }
        }
        else if (name.rfind("Paredes", 0) == 0 || name.rfind("Piso", 0) == 0) colliders.push_back(&sceneObjects[name]);
        else if (name.find("Portal") != std::string::npos) portalObject = &sceneObjects[name];
    }
    for (auto const& [num, name] : baseNames) {
        if (lidNames.count(num)) {
            auto chest = std::make_unique<Chest>();
            chest->base = &sceneObjects[name];
            chest->lid = &sceneObjects[lidNames[num]];
            chest->base_name = name;
            chest->update(0);
            chests.push_back(std::move(chest));
        }
    }
}

// ============================================================================
// PROCESSAMENTO DE INPUT
// ============================================================================
void Game::ProcessInput(float dt)
{
    // Sair do jogo com ESC
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) IsRunning = false;
    float cameraSpeed = 5.0f * dt;
    glm::vec3 moveDir(0.0f);
    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) moveDir += cameraFront;
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) moveDir -= cameraFront;
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) moveDir -= glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) moveDir += glm::normalize(glm::cross(cameraFront, cameraUp));
    moveDir.y = 0;
    if (glm::length(moveDir) > 0.0f) { moveDir = glm::normalize(moveDir) * cameraSpeed; }
    if (!checkWallCollision(cameraPos + glm::vec3(moveDir.x, 0.0f, 0.0f))) { cameraPos.x += moveDir.x; }
    if (!checkWallCollision(cameraPos + glm::vec3(0.0f, 0.0f, moveDir.z))) { cameraPos.z += moveDir.z; }
    if (cameraPos.y < 1.5f) { cameraPos.y = 1.5f; }
}

bool Game::checkWallCollision(glm::vec3 futurePos)
{
    float playerRadius = 0.4f;
    for (const auto& wall : colliders) {
        if (futurePos.x + playerRadius > wall->boundingBoxMin.x && futurePos.x - playerRadius < wall->boundingBoxMax.x &&
            futurePos.y > wall->boundingBoxMin.y && futurePos.y < wall->boundingBoxMax.y &&
            futurePos.z + playerRadius > wall->boundingBoxMin.z && futurePos.z - playerRadius < wall->boundingBoxMax.z)
        { return true; }
    }
    return false;
}

// ============================================================================
// ATUALIZAÇÃO DO JOGO
// ============================================================================
void Game::Update(float dt)
{
    // Verificar se a janela deve fechar
    if (glfwWindowShouldClose(Window)) IsRunning = false;
    
    // Atualizar animações dos baús
    for (auto& chest : chests) { 
        chest->update(dt); 
    }
    
    // Atualizar timer da mensagem da UI
    if (uiMessageTimer > 0.0f) { 
        uiMessageTimer -= dt; 
    }
}

// ============================================================================
// RENDERIZAÇÃO PRINCIPAL
// ============================================================================
void Game::Render()
{
    // Limpar buffers de cor e profundidade
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    SceneShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    SceneShader->setMat4("projection", projection);
    SceneShader->setMat4("view", view);
    
    // ===== CONFIGURAÇÃO DE ILUMINAÇÃO =====
    SceneShader->setVec3("viewPos", cameraPos);
    SceneShader->setVec3("objectColor", glm::vec3(0.6f, 0.5f, 0.4f)); // Cor base dos objetos
    
    // Luz direcional (simula luz solar)
    SceneShader->setVec3("dirLight.direction", glm::vec3(-0.5f, -1.0f, -0.5f));
    SceneShader->setVec3("dirLight.color", glm::vec3(0.8f, 0.8f, 0.8f));

    Chest* activeLightChest = nullptr;
    for (auto& chest : chests) {
        if (chest->currentLightIntensity > 0.0f) {
            activeLightChest = chest.get();
            break; 
        }
    }
    if (activeLightChest) {
        SceneShader->setVec3("chestLight.position", activeLightChest->getLightWorldPosition());
        SceneShader->setVec3("chestLight.color", activeLightChest->lightColor);
        SceneShader->setFloat("chestLight.intensity", activeLightChest->currentLightIntensity);
        // Parâmetros de atenuação para luz com alcance maior
        SceneShader->setFloat("chestLight.constant", 1.0f);
        SceneShader->setFloat("chestLight.linear", 0.05f);
        SceneShader->setFloat("chestLight.quadratic", 0.01f);
    } else {
        SceneShader->setFloat("chestLight.intensity", 0.0f);
    }

    // ===== RENDERIZAÇÃO DOS OBJETOS DA CENA =====
    for (auto const& [name, object] : sceneObjects) {
        SceneShader->setMat4("model", object.modelMatrix);
        
        // Renderização com cores sólidas (sem texturas)
        SceneShader->setInt("useTexture", 0);
        
        glBindVertexArray(object.vao);
        glDrawArrays(GL_TRIANGLES, 0, object.vertexCount);
    }
    
    // ===== INTERFACE DO USUÁRIO =====
    glDisable(GL_DEPTH_TEST);
    
    // Contador de baús abertos
    std::string counterText = "Baús abertos: " + std::to_string(chestsOpenedCount) + "/" + std::to_string(CHESTS_TO_WIN);
    Text->RenderText(counterText, 25.0f, 25.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    if (portalIsActive && !gameWon) {
        if (fmod((float)glfwGetTime(), 1.0f) > 0.5f) {
            Text->RenderText("Procure o portal!", (Width / 2.0f) - 200.0f, Height - 50.0f, 0.7f, glm::vec3(0.3f, 1.0f, 0.3f));
        }
    }
    if (gameWon) {
        Text->RenderText("FIM", (Width / 2.0f) - 70.0f, Height / 2.0f, 2.0f, glm::vec3(1.0f, 0.9f, 0.2f));
    }
    else if (uiMessageTimer > 0.0f) {
        Text->RenderText(uiMessage, (Width / 2.0f) - 300.0f, Height / 2.0f, 0.7f, glm::vec3(1.0f, 0.2f, 0.2f));
    }
    glfwSwapBuffers(Window);
    glfwPollEvents();
}

void Game::FramebufferSizeCallback(int width, int height) { glViewport(0, 0, width, height); }

void Game::MouseCallback(double xpos, double ypos)
{
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void Game::MouseButtonCallback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        handleInteraction();
    }
}

void Game::handleInteraction()
{
    if (gameWon) return;
    const float interaction_range = 3.5f;
    if (portalObject != nullptr) {
        glm::vec3 portalCenter = (portalObject->boundingBoxMin + portalObject->boundingBoxMax) / 2.0f;
        float distanceToPortal = glm::distance(cameraPos, portalCenter);
        if (distanceToPortal < interaction_range) {
            if (chestsOpenedCount >= CHESTS_TO_WIN) {
                gameWon = true;
            } else {
                int remaining = CHESTS_TO_WIN - chestsOpenedCount;
                uiMessage = "Faltam " + std::to_string(remaining) + " baus para abrir o portal!";
                uiMessageTimer = 3.0f;
            }
            return;
        }
    }
    float closest_distance = std::numeric_limits<float>::max();
    Chest* target_chest = nullptr;
    if (chests.empty()) { return; }
    for (auto& chest : chests) {
        glm::vec3 chestCenter = (chest->base->boundingBoxMin + chest->base->boundingBoxMax) / 2.0f;
        float distance = glm::distance(cameraPos, chestCenter);
        if (distance < closest_distance) {
            closest_distance = distance;
            target_chest = chest.get();
        }
    }
    if (target_chest && closest_distance < interaction_range) {
        if (target_chest->toggleOpen()) {
            chestsOpenedCount++;
            if (chestsOpenedCount >= CHESTS_TO_WIN) {
                portalIsActive = true;
            }
        }
    }
}

// --- Implementação dos métodos da struct Chest ---
bool Chest::toggleOpen()
{
    isOpen = !isOpen;
    targetLidOffsetY = isOpen ? 0.7f : 0.0f;
    isAnimating = true;
    if (isOpen && !hasBeenCounted) {
        hasBeenCounted = true;
        return true;
    }
    return false;
}

void Chest::update(float dt)
{
    // Atualizar animação da tampa
    if (isAnimating) {
        if (std::abs(lidOffsetY - targetLidOffsetY) < 0.01f) { 
            lidOffsetY = targetLidOffsetY; 
            isAnimating = false; 
        }
        else if (lidOffsetY < targetLidOffsetY) { 
            lidOffsetY += animationSpeed * dt; 
        }
        else { 
            lidOffsetY -= animationSpeed * dt; 
        }
        lid->modelMatrix = base->modelMatrix;
        lid->modelMatrix = glm::translate(lid->modelMatrix, glm::vec3(0.0f, lidOffsetY, 0.0f));
    }
    
    // Atualizar intensidade da luz baseada no estado de abertura
    if (isOpen) {
        targetLightIntensity = 2.5f; // Luz forte quando aberto
    } else {
        targetLightIntensity = 0.0f; // Sem luz quando fechado
    }
    
    // Interpolar intensidade da luz suavemente
    if (currentLightIntensity < targetLightIntensity) {
        currentLightIntensity += lightFadeSpeed * dt;
        if (currentLightIntensity > targetLightIntensity) {
            currentLightIntensity = targetLightIntensity;
        }
    } else if (currentLightIntensity > targetLightIntensity) {
        currentLightIntensity -= lightFadeSpeed * dt;
        if (currentLightIntensity < targetLightIntensity) {
            currentLightIntensity = targetLightIntensity;
        }
    }
}

glm::vec3 Chest::getLightWorldPosition() const
{
    glm::vec3 chestCenter = (base->boundingBoxMin + base->boundingBoxMax) / 2.0f;
    // Posicionar a luz no centro do baú, ligeiramente acima
    return chestCenter + glm::vec3(0.0f, 0.3f, 0.0f);
}


