# Labirinto 3D Interativo em OpenGL

Este projeto é um jogo de exploração de um labirinto 3D desenvolvido em C++ com a biblioteca gráfica OpenGL. O objetivo é navegar pelo labirinto, encontrar e abrir 3 baús para ativar um portal e chegar ao final.

## Funcionalidades

* **Câmara em Primeira Pessoa:** Movimentação livre pelo cenário com controlos padrão (WASD + Mouse).
* **Colisão com o Cenário:** Um sistema de colisão baseado em caixas delimitadoras (AABB) impede que o jogador atravesse paredes e o chão.
* **Objetos Interativos:** Os baús podem ser abertos através da proximidade e de um clique, ativando uma animação de "levitação" da tampa.
* **Sistema de Jogo:** O objetivo é abrir 3 baús para ativar um portal. Ao interagir com o portal, o jogo exibe mensagens de estado.
* **Interface de Utilizador (UI):** O display mostra um contador de baús abertos e mensagens dinâmicas, como o objetivo atual ("Procure o portal!") e a mensagem de vitória ("FIM").

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

---

## Como Compilar e Executar

As instruções variam dependendo do sistema operativo.

### 🐧 Instruções para Linux (Debian/Ubuntu)

#### 1. Instalar Dependências

Abra um terminal e execute o seguinte comando para instalar tudo o que é necessário:
```bash
sudo apt-get update && sudo apt-get install build-essential cmake libglfw3-dev libglm-dev libfreetype-dev git
```

#### 2. Obter o Código

Clone este repositório para o seu computador:
```bash
# Substitua o URL pelo URL do seu repositório no GitHub
git clone [https://github.com/AntonioCVaz/Labirinto-OpenGL.git](https://github.com/AntonioCVaz/Labirinto-OpenGL.git)
cd Labirinto-OpenGL
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

* **Visual Studio 2022:** Instale a partir do [site oficial](https://visualstudio.microsoft.com/pt-br/vs/). Durante a instalação, certifique-se de que marca a carga de trabalho **"Desenvolvimento de desktop com C++"**.
* **Git for Windows:** Instale a partir do [site oficial](https://git-scm.com/download/win).
* **CMake:** Instale a partir do [site oficial](https://cmake.org/download/). Certifique-se de que, durante a instalação, escolhe a opção para adicionar o CMake ao PATH do sistema para todos os utilizadores.

#### 2. Instalar Dependências com vcpkg

O `vcpkg` irá descarregar e compilar as bibliotecas necessárias para nós.

Abra um terminal **PowerShell como Administrador** e execute os seguintes comandos, um de cada vez:

```powershell
# Navega para um diretório onde queira instalar o vcpkg (ex: C:\src)
mkdir C:\src
cd C:\src

# Clona o repositório do vcpkg
git clone [https://github.com/Microsoft/vcpkg.git](https://github.com/Microsoft/vcpkg.git)

# Entra na pasta e executa o script de bootstrap
cd vcpkg
.\bootstrap-vcpkg.bat

# Integra o vcpkg com o Visual Studio (passo importante!)
.\vcpkg integrate install

# Instala as bibliotecas que o nosso projeto precisa
.\vcpkg install glfw3 glm freetype
```

#### 3. Obter e Compilar o Projeto

1.  Abra o **Git Bash** (que foi instalado com o Git for Windows).
2.  Navegue para a pasta onde quer guardar o projeto (ex: `cd Documents`).
3.  Clone o repositório:
    ```bash
    # Substitua o URL pelo URL do seu repositório no GitHub
    git clone [https://github.com/AntonioCVaz/Labirinto-OpenGL.git](https://github.com/AntonioCVaz/Labirinto-OpenGL.git)
    ```
4.  Abra o **Visual Studio 2022**.
5.  Vá a `File` > `Open` > `CMake...` e selecione o ficheiro `CMakeLists.txt` na pasta `Labirinto-OpenGL` que acabou de clonar.
6.  O Visual Studio irá detetar automaticamente as bibliotecas instaladas pelo `vcpkg` e configurar o projeto.
7.  No menu superior, vá a `Build` > `Build All`.

#### 4. Executar

Após a compilação, pode executar o projeto diretamente a partir do Visual Studio:
* Vá ao menu superior e clique em `Debug` > `Start Without Debugging` (ou prima `Ctrl+F5`).

---

## Controles

* **W, A, S, D**: Mover a câmara.
* **Mouse**: Olhar ao redor.
* **Clique Esquerdo**: Interagir com o baú ou portal mais próximo.
* **ESC**: Fechar o programa.
