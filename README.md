# Labirinto 3D Interativo em OpenGL

Este projeto é um jogo de exploração de um labirinto 3D desenvolvido em C++ com a biblioteca gráfica OpenGL. O objetivo é navegar pelo labirinto, encontrar e abrir 3 baús para ativar um portal e chegar ao final.

## Funcionalidades

* **Câmera em Primeira Pessoa:** Movimentação livre pelo cenário com controles padrão (WASD + Mouse).
* **Colisão com o Cenário:** Um sistema de colisão baseado em bounding boxes (AABB) impede que o jogador atravesse paredes e o chão.
* **Objetos Interativos:** Os baús podem ser abertos por proximidade e de um clique, ativando uma animação de "levitação" da tampa.
* **Sistema de Jogo:** O objetivo é abrir 3 baús para ativar um portal. Ao interagir com o portal, o jogo exibe mensagens de estado.
* **Interface de Usuário (UI):** O display mostra um contador de baús abertos e mensagens dinâmicas, como o objetivo atual ("Procure o portal!") e a mensagem de vitória ("FIM").

## Tecnologias Utilizadas

* **Linguagem:** C++17
* **Gráficos:** OpenGL 3.3 (Core Profile)
* **Bibliotecas Principais:**
    * **GLFW:** Para criação de janela, contexto OpenGL e gestão de inputs.
    * **GLAD:** Para carregar as funções do OpenGL.
    * **GLM:** Para matemática de vetores e matrizes.
    * **tinyobjloader:** Para carregar modelos 3D no formato `.obj`.
    * **Freetype:** Para renderização de texto e da UI na tela.
* **Modelagem 3D:** Blender
* **Sistema de Compilação:** CMake

---

## Como Compilar e Executar

As instruções variam dependendo do sistema operacional.

### 🐧 Instruções para Linux (Debian/Ubuntu)

#### 1. Instalar Dependências

Abra um terminal e execute o seguinte comando para instalar tudo o que é necessário:
```bash
sudo apt-get update && sudo apt-get install build-essential cmake libglfw3-dev libglm-dev libfreetype-dev git
```

#### 2. Obter o Código

Clone este repositório, mas antes, navegue até a pasta que deseja armazená-lo:
```bash
cd ~/pasta-desejada
git clone https://github.com/cauaofduty/labyrinth-cg-2025.1.git

```

#### 3. Compilar o Projeto

Dentro da pasta do projeto, execute os seguintes comandos:
```bash
# Cria uma pasta de compilação e entra nela
mkdir build && cd build

# Configura o projeto com CMake
cmake ..

# Compila o código
make
```

#### 4. Executar

Após a compilação, ainda dentro da pasta `build`, execute:
```bash
./PROJETO_CG
```

---

### 🪟 Instruções para Windows

No Windows, o processo é mais fácil usando o **Visual Studio** e o gestor de pacotes **vcpkg**.

#### 1. Instalar Ferramentas

* **Visual Studio 2022:** Instale a partir do [site oficial](https://visualstudio.microsoft.com/pt-br/vs/). Durante a instalação, certifique-se de que seleciona o workload **"Desenvolvimento de desktop com C++"**.
* **Git for Windows:** Instale a partir do [site oficial](https://git-scm.com/download/win).
* **CMake:** Instale a partir do [site oficial](https://cmake.org/download/). Certifique-se de que, durante a instalação, escolha a opção para adicionar o CMake ao PATH do sistema para todos os usuários.

#### 2. Instalar Dependências com vcpkg

O `vcpkg` irá baixar e compilar as bibliotecas necessárias para nós.

Abra um terminal **PowerShell como Administrador** e execute os seguintes comandos, um de cada vez:

```powershell
# Navegue para a pasta qual queira instalar o vcpkg (ex: C:\src)
mkdir C:\src
cd C:\src

# Clone o repositório do vcpkg
git clone https://github.com/Microsoft/vcpkg.git](https://github.com/Microsoft/vcpkg.git

# Entre na pasta e execute o script de bootstrap
cd vcpkg
.\bootstrap-vcpkg.bat

# Integre o vcpkg com o Visual Studio (passo importante!)
.\vcpkg integrate install

# Instale as bibliotecas que o projeto precisa
.\vcpkg install glfw3 glm freetype
```

#### 3. Obter e Compilar o Projeto

1.  Abra o **Git Bash** (que foi instalado com o Git for Windows).
2.  Navegue para a pasta que deve armazenar o projeto (ex: `cd Documents`).
3.  Clone o repositório:
    ```bash
     git clone https://github.com/cauaofduty/labyrinth-cg-2025.1.git

    ```
4.  Abra o **Visual Studio 2022**.
5.  Vá em `File` > `Open` > `CMake...` e selecione o arquivo `CMakeLists.txt` na pasta `Labirinto-OpenGL` que acabou de clonar.
6.  O Visual Studio irá detectar automaticamente as bibliotecas instaladas pelo `vcpkg` e configurar o projeto.
7.  No menu superior, vá em `Build` > `Build All`.

#### 4. Executar

Após a compilação, pode executar o projeto diretamente a partir do Visual Studio:
* Vá ao menu superior e clique em `Debug` > `Start Without Debugging` (ou pelas teclas `Ctrl+F5`).

---

## Controles

* **W, A, S, D**: Mover a câmera.
* **Mouse**: Olhar ao redor.
* **Clique Esquerdo**: Interagir com o baú ou portal mais próximo.
* **ESC**: Fechar o programa.
