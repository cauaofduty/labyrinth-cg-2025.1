#include "Game.h"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    std::cout << "Iniciando jogo..." << std::endl;
    
    Game Labirinto(SCR_WIDTH, SCR_HEIGHT);
    if (!Labirinto.IsRunning) {
        std::cout << "Falha na inicialização do Game" << std::endl;
        return -1;
    }
    
    std::cout << "Chamando Init()..." << std::endl;
    Labirinto.Init();
    std::cout << "Init() concluído" << std::endl;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (Labirinto.IsRunning)
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Labirinto.ProcessInput(deltaTime);
        Labirinto.Update(deltaTime);
        Labirinto.Render();
    }

    return 0;
}
