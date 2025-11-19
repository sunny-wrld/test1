cmake_minimum_required(VERSION 3.22.1)
project("llama")

# Configurações
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Diretórios
# Nota: Ajuste este caminho conforme a localização do llama.cpp no seu projeto
# Se você não tiver llama.cpp, comente as seções relacionadas e defina LLAMA_CPP_AVAILABLE=0
set(LLAMA_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/llama.cpp)

# Verifica se llama.cpp está disponível
if(EXISTS ${LLAMA_CPP_DIR}/llama.h)
    add_definitions(-DLLAMA_CPP_AVAILABLE)
    
    # Inclui diretórios
    include_directories(
        ${LLAMA_CPP_DIR}
        ${LLAMA_CPP_DIR}/include
    )
    
    # Arquivos fonte do llama.cpp (simplificado - você precisará incluir todos)
    # Nota: A lista completa de arquivos depende da versão do llama.cpp
    set(LLAMA_SOURCES
        ${LLAMA_CPP_DIR}/ggml.c
        ${LLAMA_CPP_DIR}/ggml-alloc.c
        ${LLAMA_CPP_DIR}/ggml-backend.c
        ${LLAMA_CPP_DIR}/llama.cpp
        # Adicione outros arquivos necessários do llama.cpp conforme a versão
    )
    
    # Biblioteca nativa com llama.cpp
    add_library(llama SHARED
        llama_jni.cpp
        ${LLAMA_SOURCES}
    )
else()
    # Biblioteca nativa sem llama.cpp (stub)
    add_library(llama SHARED
        llama_jni.cpp
    )
    message(WARNING "llama.cpp não encontrado em ${LLAMA_CPP_DIR}. Compilando sem suporte nativo.")
endif()

# Linka bibliotecas
find_library(log-lib log)
target_link_libraries(llama ${log-lib})

# Flags de compilação
# Removido -march=native para compatibilidade entre diferentes arquiteturas
target_compile_options(llama PRIVATE -O3)



