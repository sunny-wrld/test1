# Aurora Edge - Documentação Técnica Completa

## 1. Visão Geral da Arquitetura

### 1.1 Conceito

Aurora Edge é uma solução de assistente de IA que executa modelos de linguagem grandes (LLMs) quantizados diretamente em dispositivos Android, sem dependência de serviços em nuvem ou conexão com a internet.

### 1.2 Princípios de Design

- **Offline-First**: Todas as operações são executadas localmente
- **Privacidade por Design**: Dados nunca saem do dispositivo
- **Eficiência**: Uso de modelos quantizados para reduzir requisitos de hardware
- **Modularidade**: Arquitetura extensível para suportar múltiplos backends

## 2. Arquitetura do Sistema

### 2.1 Diagrama de Camadas

```
┌─────────────────────────────────────────┐
│         CAMADA DE APRESENTAÇÃO          │
│  (Activities, Fragments, ViewModels)    │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│      CAMADA DE LÓGICA DE NEGÓCIO        │
│    (AIModelManager, TextProcessor)      │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│      CAMADA DE INFERÊNCIA NATIVA        │
│      (JNI Bridge, Llama.cpp/ONNX)       │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│      CAMADA DE ARMAZENAMENTO            │
│   (Modelos GGUF, Cache, Histórico)      │
└─────────────────────────────────────────┘
```

### 2.2 Componentes Principais

#### 2.2.1 Camada de Apresentação

- **ChatActivity**: Interface principal de chat
- **SettingsActivity**: Configurações do app e modelo
- **ChatAdapter**: RecyclerView adapter para mensagens
- **ViewModels**: Gerenciamento de estado (futuro)

#### 2.2.2 Camada de Lógica

- **AIModelManager**: Gerenciamento do ciclo de vida do modelo
  - Carregamento de modelos GGUF
  - Execução de inferência
  - Gerenciamento de memória
  - Funções utilitárias (resumo, tradução)

#### 2.2.3 Camada Nativa

- **llama_jni.cpp**: Bridge JNI para Llama.cpp
  - `loadModelNative()`: Carrega modelo GGUF
  - `generateResponseNative()`: Executa inferência
  - `releaseModelNative()`: Libera recursos

#### 2.2.4 Camada de Armazenamento

- **Armazenamento Interno**: Modelos em `/data/data/com.auroraedge.app/files/models/`
- **Cache**: Histórico de conversas (futuro)
- **SharedPreferences**: Configurações do usuário

## 3. Fluxo de Funcionamento

### 3.1 Inicialização do App

```
1. Usuário abre app
   ↓
2. ChatActivity.onCreate()
   ↓
3. AIModelManager.initialize()
   ↓
4. Verifica se modelo existe no armazenamento
   ↓
5. Carrega modelo via JNI (llama.cpp)
   ↓
6. Modelo pronto → Habilita interface
```

### 3.2 Fluxo de Inferência

```
1. Usuário digita mensagem
   ↓
2. ChatActivity.sendMessage()
   ↓
3. Adiciona mensagem do usuário ao chat
   ↓
4. AIModelManager.generateResponse()
   ↓
5. Constrói prompt com contexto
   ↓
6. Chama generateResponseNative() via JNI
   ↓
7. Llama.cpp executa inferência:
   - Tokenização do prompt
   - Forward pass através do modelo
   - Sampling (temperatura, top-p)
   - Decodificação de tokens
   ↓
8. Retorna resposta gerada
   ↓
9. Adiciona resposta ao chat
```

### 3.3 Fluxo de Resumo de Texto

```
1. Usuário seleciona texto
   ↓
2. Chama AIModelManager.summarizeText()
   ↓
3. Constrói prompt especializado:
   "Resuma o seguinte texto..."
   ↓
4. Executa inferência (mesmo fluxo acima)
   ↓
5. Retorna resumo
```

## 4. Estrutura do Motor de IA Local

### 4.1 Modelos Suportados

#### 4.1.1 Formatos

- **GGUF (GGML Universal Format)**: Formato principal
  - Quantizações: Q4_0, Q4_1, Q5_0, Q5_1, Q8_0, Q2_K, Q3_K_M
  - Suporta: Llama, Mistral, Phi, Gemma, CodeLlama

- **ONNX Runtime Mobile** (futuro)
  - Modelos ONNX quantizados
  - Suporte a GPU via NNAPI

#### 4.1.2 Modelos Recomendados

| Modelo | Tamanho | RAM Mínima | Qualidade | Velocidade |
|--------|---------|------------|-----------|------------|
| Phi-2 Q4_0 | 1.6GB | 2GB | Alta | Rápida |
| Gemma 2B Q4_0 | 1.4GB | 2GB | Alta | Rápida |
| Mistral 7B Q4_0 | 4GB | 4GB | Muito Alta | Média |
| Llama 3B Q4_0 | 2GB | 3GB | Alta | Média |

### 4.2 Execução de Modelos Quantizados

#### 4.2.1 Quantização

A quantização reduz a precisão dos pesos do modelo:
- **FP32** (32-bit float): Original, ~4 bytes/parâmetro
- **Q4_0** (4-bit): ~0.5 bytes/parâmetro, 8x menor
- **Q2_K** (2-bit): ~0.25 bytes/parâmetro, 16x menor

#### 4.2.2 Processo de Inferência

1. **Carregamento**: Modelo GGUF é mapeado na memória
2. **Tokenização**: Texto → Tokens (vocabulário do modelo)
3. **Embedding**: Tokens → Vetores de embedding
4. **Forward Pass**: 
   - Através de camadas de atenção
   - Através de camadas feed-forward
   - Aplicação de normalização
5. **Sampling**: Seleção do próximo token
   - Temperature: Controla aleatoriedade
   - Top-p: Nucleus sampling
   - Top-k: Amostragem dos k melhores
6. **Decodificação**: Tokens → Texto

### 4.3 Requisitos Técnicos

#### 4.3.1 CPU

- **Mínimo**: ARMv7 (32-bit) ou ARM64 (64-bit)
- **Recomendado**: ARM64 com 4+ cores
- **Otimização**: Suporte a NEON (SIMD)

#### 4.3.2 RAM

- **Mínimo**: 2GB total, 1GB livre
- **Recomendado**: 4GB+ total, 2GB+ livre
- **Uso típico**: 1.5-3GB para modelo + sistema

#### 4.3.3 Armazenamento

- **Modelo**: 500MB - 4GB (dependendo do modelo)
- **App**: 50MB
- **Cache**: 10-100MB (futuro)

#### 4.3.4 GPU/NPU (Opcional)

- **NNAPI**: Aceleração via GPU/NPU do Android
- **Vulkan**: Suporte futuro para GPU dedicada
- **Benefício**: 2-5x mais rápido em dispositivos compatíveis

## 5. Armazenamento de Dados Offline

### 5.1 Estrutura de Diretórios

```
/data/data/com.auroraedge.app/
├── files/
│   ├── models/
│   │   ├── phi-2-q4_0.gguf
│   │   ├── gemma-2b-q4_0.gguf
│   │   └── ...
│   ├── cache/
│   │   ├── conversations/
│   │   └── embeddings/
│   └── logs/
├── shared_prefs/
│   └── aurora_prefs.xml
└── databases/
    └── chat_history.db (futuro)
```

### 5.2 Modelos

- Localização: `files/models/`
- Formato: GGUF
- Carregamento: Lazy loading (apenas quando necessário)
- Cache: Mantido em memória durante uso

### 5.3 Histórico de Conversas (Futuro)

- Banco de dados SQLite local
- Criptografia AES-256
- Sem sincronização com nuvem

### 5.4 Configurações

- SharedPreferences
- Chaves:
  - `model_path`: Caminho do modelo atual
  - `max_tokens`: Máximo de tokens por resposta
  - `temperature`: Temperatura de sampling
  - `theme`: Tema da interface

## 6. Casos de Uso

### 6.1 Saúde

- **Análise de Sintomas**: Assistente médico offline
- **Privacidade de Dados**: Informações sensíveis nunca saem do dispositivo
- **Áreas Remotas**: Funciona sem internet
- **Conformidade**: LGPD/GDPR compliant

### 6.2 Educação

- **Tutoria Offline**: Ajuda com lições de casa
- **Resumo de Textos**: Análise de artigos e livros
- **Tradução**: Suporte a múltiplos idiomas
- **Acessibilidade**: Disponível em áreas sem internet

### 6.3 Governo

- **Atendimento Cidadão**: Chatbots em postos públicos
- **Segurança**: Dados governamentais não vão para nuvem
- **Soberania**: Tecnologia nacional, sem dependência externa
- **Custo**: Reduz custos de infraestrutura

## 7. Roadmap Futuro

### 7.1 Fase 2 - Agentes Autônomos

- **Agentes Locais**: Execução de tarefas complexas
- **Planejamento**: Quebra de problemas em subtarefas
- **Memória Persistente**: Lembrança de interações anteriores
- **Tool Calling**: Integração com APIs locais

### 7.2 Fase 3 - Multimodal

- **Visão**: Análise de imagens offline
- **Voz**: Reconhecimento e síntese de voz local
- **OCR**: Leitura de documentos
- **Análise de PDF**: Extração e resumo

### 7.3 Fase 4 - Automações

- **Automação de Tarefas**: Integração com Tasker/IFTTT
- **Widgets**: Respostas rápidas na tela inicial
- **Notificações Inteligentes**: Análise de notificações
- **Assistente Pessoal**: Agendamento, lembretes

### 7.4 Fase 5 - Expansões

- **Plugins Locais**: Extensões sem nuvem
- **Wearables**: Suporte a smartwatches
- **IoT**: Integração com dispositivos locais
- **Edge Computing**: Distribuição em rede local

## 8. Diagramas ASCII

### 8.1 Arquitetura de Componentes

```
┌──────────────┐
│   Usuário    │
└──────┬───────┘
       │
       ▼
┌─────────────────────────────────┐
│      ChatActivity (UI)          │
│  - Input de mensagem            │
│  - Exibição de respostas        │
│  - Gerenciamento de estado      │
└──────────────┬──────────────────┘
               │
               ▼
┌─────────────────────────────────┐
│     AIModelManager (Kotlin)     │
│  - Carregamento de modelo       │
│  - Construção de prompts        │
│  - Gerenciamento de memória     │
└──────────────┬──────────────────┘
               │ JNI
               ▼
┌─────────────────────────────────┐
│    llama_jni.cpp (C++)          │
│  - Bridge JNI                   │
│  - Chamadas nativas             │
└──────────────┬──────────────────┘
               │
               ▼
┌─────────────────────────────────┐
│      Llama.cpp (C++)            │
│  - Carregamento GGUF            │
│  - Inferência                   │
│  - Sampling                     │
└──────────────┬──────────────────┘
               │
               ▼
┌─────────────────────────────────┐
│   Modelo GGUF (Disco)           │
│  - Pesos quantizados            │
│  - Vocabulário                  │
└─────────────────────────────────┘
```

### 8.2 Fluxo de Dados

```
[Usuário] → "Olá, como você está?"
    │
    ▼
[ChatActivity] → Adiciona ao chat
    │
    ▼
[AIModelManager] → Constrói prompt:
    │              "Você é Aurora Edge...
    │               Usuário: Olá, como você está?
    │               Aurora:"
    │
    ▼
[JNI Bridge] → Passa para C++
    │
    ▼
[Llama.cpp] → Tokenização
    │         "Você" → [1234]
    │         "é" → [567]
    │         ...
    │
    ▼
[Forward Pass] → Através do modelo
    │            - Attention layers
    │            - Feed-forward
    │            - Normalization
    │
    ▼
[Sampling] → Seleciona próximo token
    │         Temperature: 0.7
    │         Top-p: 0.9
    │
    ▼
[Decodificação] → Token → "Olá"
    │
    ▼
[Geração] → Repete até EOS ou max_tokens
    │         "Olá! Estou bem, obrigado..."
    │
    ▼
[Retorno] → Kotlin → UI
    │
    ▼
[ChatActivity] → Exibe resposta
    │
    ▼
[Usuário] ← "Olá! Estou bem, obrigado..."
```

## 9. Especificação Técnica

### 9.1 APIs Principais

#### AIModelManager

```kotlin
class AIModelManager(context: Context) {
    fun loadModel(): Boolean
    fun generateResponse(prompt: String): String
    fun summarizeText(text: String): String
    fun isModelLoaded(): Boolean
    fun release()
}
```

#### JNI Interface

```cpp
// Carrega modelo
jlong loadModelNative(JNIEnv* env, jobject thiz, jstring modelPath);

// Gera resposta
jstring generateResponseNative(
    JNIEnv* env, 
    jobject thiz, 
    jlong handle, 
    jstring prompt, 
    jint maxTokens, 
    jfloat temperature, 
    jfloat topP
);

// Libera recursos
void releaseModelNative(JNIEnv* env, jobject thiz, jlong handle);
```

### 9.2 Parâmetros de Inferência

- **max_tokens**: 512 (padrão)
- **temperature**: 0.7 (padrão)
- **top_p**: 0.9 (padrão)
- **top_k**: 40 (padrão)
- **repeat_penalty**: 1.1 (padrão)
- **context_size**: 2048 (padrão)

### 9.3 Métricas de Performance

- **Tempo de carregamento**: 30-60 segundos (modelo 1.6GB)
- **Latência de primeira resposta**: 2-5 segundos
- **Tokens por segundo**: 5-15 (CPU), 20-50 (GPU)
- **Uso de RAM**: 1.5-3GB (dependendo do modelo)
- **Uso de CPU**: 50-100% durante inferência

## 10. Segurança e Privacidade

### 10.1 Garantias

- ✅ **Zero Telemetria**: Nenhum dado enviado para servidores
- ✅ **Armazenamento Local**: Todos os dados no dispositivo
- ✅ **Sem Permissões de Rede**: App não pode acessar internet
- ✅ **Criptografia**: Histórico criptografado (futuro)

### 10.2 Conformidade

- **LGPD**: Compatível (dados não saem do dispositivo)
- **GDPR**: Compatível (privacidade por design)
- **HIPAA**: Potencialmente compatível (com auditoria)

## 11. Limitações Atuais

1. **Tamanho do Modelo**: Modelos grandes (>7B) podem não rodar em dispositivos básicos
2. **Velocidade**: Mais lento que soluções em nuvem (mas mais privado)
3. **Qualidade**: Modelos quantizados podem ter qualidade ligeiramente inferior
4. **Multimodal**: Ainda não suporta imagens/voz (roadmap)

## 12. Conclusão

Aurora Edge representa uma abordagem inovadora para IA no edge, priorizando privacidade, acessibilidade e soberania tecnológica. A arquitetura modular permite evolução contínua, com roadmap claro para funcionalidades avançadas.

---

**Versão**: 1.0.0  
**Data**: 2024  
**Autor**: Equipe Aurora Edge



