# Labirinto 3D Interativo em OpenGL

![Demonstração do Jogo](caminho/para/sua/imagem.gif)  Este projeto é um jogo de exploração de um labirinto 3D desenvolvido em C++ com a biblioteca gráfica OpenGL. O objetivo é navegar pelo labirinto, encontrar e abrir 3 baús para ativar um portal e chegar ao final.

## Funcionalidades

* **Câmara em Primeira Pessoa:** Movimentação livre pelo cenário com controlos padrão (WASD + Mouse).
* **Colisão com o Cenário:** Um sistema de colisão baseado em caixas delimitadoras (AABB) impede que o jogador atravesse paredes e o chão.
* **Objetos Interativos:** Os baús podem ser abertos através da proximidade e de um clique, ativando uma animação de "levitação" da tampa.
* **Sistema de Jogo:** O objetivo é abrir 3 baús para ativar um portal, que ao ser alcançado, finaliza o jogo.
* **Interface de Utilizador (UI):** O ecrã mostra um contador de baús abertos e mensagens de estado (ex: "Procure o portal!", "FIM").

## Tecnologias Utilizadas

* **Linguagem:** C++17
* **Gráficos:** OpenGL 3.3 (Core Profile)
* **Bibliotecas Principais:**
    * **GLFW:** Para criação de janela, contexto OpenGL e gestão de inputs.
    * **GLAD:** Para carregar as funções do OpenGL.
    * **GLM:** Para matemática de vetores e matrizes.
    * **tinyobjloader:** Para carregar modelos 3D no formato `.obj`.
    * **Freetype:** Para renderização de texto e da UI no ecrã.
* **Modelação 3D:** Blender
* **Sistema de Compilação:** CMake

## Como Compilar e Executar (Linux - Debian/Ubuntu)

### 1. Instalar Dependências

Primeiro, instale as bibliotecas necessárias. Abra um terminal e execute:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libglfw3-dev libglm-dev libfreetype-dev
