cmake_minimum_required(VERSION 3.10)
project(OpenGL_Labirinto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
endif()

cmake_policy(SET CMP0072 NEW)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

# Adiciona a pasta src aos diretórios de include para que possamos fazer #include "Shader.h"
include_directories(include src)

# CORREÇÃO: Lista explicitamente todos os ficheiros fonte a serem compilados
add_executable(PROJETO_CG 
    src/main.cpp 
    src/glad.c
    src/Game.cpp
    src/Shader.cpp
    src/TextRenderer.cpp
)

# "Linka" (conecta) seu programa com as bibliotecas
target_link_libraries(PROJETO_CG PRIVATE glfw ${OPENGL_LIBRARIES} Freetype::Freetype)

# Copia as pastas de recursos para o diretório de build
file(COPY shaders models fonts DESTINATION ${CMAKE_BINARY_DIR})
