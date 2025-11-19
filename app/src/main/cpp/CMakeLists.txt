cmake_minimum_required(VERSION 3.22.1)
project("llama")

# Configurações
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Diretórios
set(LLAMA_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/llama.cpp)

# Inclui diretórios
include_directories(
    ${LLAMA_CPP_DIR}
    ${LLAMA_CPP_DIR}/include
)

# Arquivos fonte do llama.cpp (simplificado - você precisará incluir todos)
set(LLAMA_SOURCES
    ${LLAMA_CPP_DIR}/ggml.c
    ${LLAMA_CPP_DIR}/ggml-alloc.c
    ${LLAMA_CPP_DIR}/ggml-backend.c
    ${LLAMA_CPP_DIR}/llama.cpp
    # Adicione outros arquivos necessários do llama.cpp
)

# Biblioteca nativa
add_library(llama SHARED
    llama_jni.cpp
    ${LLAMA_SOURCES}
)

# Linka bibliotecas
find_library(log-lib log)
target_link_libraries(llama ${log-lib})

# Flags de compilação
target_compile_options(llama PRIVATE -O3 -march=native)


