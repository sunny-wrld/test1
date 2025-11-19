# Aurora Edge - Assistente de IA Offline

## 📱 Sobre o Projeto

Aurora Edge é um assistente de IA completamente offline para Android, que roda modelos de linguagem quantizados localmente no dispositivo, sem necessidade de conexão com a internet.

## 🚀 Guia Rápido: Testar no Celular

**Quer testar no seu celular?** Veja o **[Guia Completo de Teste](./GUIA_TESTE_CELULAR.md)** com instruções passo a passo detalhadas!

**Resumo rápido:**
1. Ative modo desenvolvedor no celular
2. Conecte via USB e instale o app
3. Baixe um modelo GGUF (ex: Phi-2 Q4_0)
4. Copie o modelo para a pasta do app
5. Ative modo avião e teste offline!

## 🚀 Características

- ✅ **100% Offline** - Zero dependência de internet
- ✅ **Privacidade Total** - Dados nunca saem do dispositivo
- ✅ **Baixa Latência** - Respostas instantâneas
- ✅ **Modelos Quantizados** - 50-300MB de tamanho
- ✅ **Suporte a GGUF** - Formatos Llama, Phi, Gemma, Mistral

## 📋 Requisitos

- Android Studio Hedgehog ou superior
- Android SDK 24+ (Android 7.0+)
- NDK r25c ou superior
- Dispositivo com pelo menos 2GB RAM
- 500MB de armazenamento livre

## 🔧 Como Compilar

### 1. Clonar o Repositório

```bash
git clone <repository-url>
cd AuroraEdge
```

### 2. Configurar Llama.cpp

```bash
# Clonar llama.cpp como submodule
git submodule add https://github.com/ggerganov/llama.cpp.git third_party/llama.cpp
git submodule update --init --recursive
```

### 3. Baixar Modelo

Baixe um modelo quantizado GGUF (recomendado: Phi-2 Q4_0):

```bash
# Criar diretório de modelos
mkdir -p app/src/main/assets/models

# Baixar modelo (exemplo com Phi-2)
wget https://huggingface.co/microsoft/phi-2-gguf/resolve/main/phi-2.Q4_0.gguf \
  -O app/src/main/assets/models/phi-2-q4_0.gguf
```

**Modelos Recomendados:**
- Phi-2 Q4_0 (~1.6GB) - Melhor qualidade
- Gemma 2B Q4_0 (~1.4GB) - Boa qualidade
- Mistral 7B Q4_0 (~4GB) - Alta qualidade (requer mais RAM)

### 4. Compilar no Android Studio

1. Abra o projeto no Android Studio
2. Aguarde a sincronização do Gradle
3. Conecte um dispositivo Android ou inicie um emulador
4. Clique em "Run" (Shift+F10)

### 5. Compilar via Linha de Comando

```bash
./gradlew assembleDebug
adb install app/build/outputs/apk/debug/app-debug.apk
```

## 🧪 Como Testar Offline

### 1. Preparar Dispositivo

```bash
# Instalar APK
adb install app-debug.apk

# Copiar modelo para o dispositivo
adb push app/src/main/assets/models/phi-2-q4_0.gguf \
  /sdcard/Android/data/com.auroraedge.app/files/models/phi-2-q4_0.gguf
```

### 2. Testar Offline

1. **Ativar Modo Avião** no dispositivo
2. Abrir o app Aurora Edge
3. Aguardar carregamento do modelo (30-60 segundos)
4. Enviar mensagem de teste: "Olá, como você está?"
5. Verificar resposta gerada localmente

### 3. Testar Função de Resumo

1. Clicar no botão "📄" (resumir)
2. Colar um texto longo
3. Verificar resumo gerado

## 🔄 Como Trocar Modelo

### Via Interface do App

1. Abrir Configurações
2. Selecionar "Modelo de IA"
3. Escolher modelo da lista
4. Aguardar carregamento

### Via Arquivos

1. Copiar novo modelo GGUF para:
   ```
   /sdcard/Android/data/com.auroraedge.app/files/models/
   ```
2. Reiniciar o app
3. O app detectará automaticamente

### Formatos Suportados

- GGUF (Q4_0, Q4_1, Q5_0, Q5_1, Q8_0)
- ONNX (futuro)

## ⚡ Otimizações de Desempenho

### 1. Reduzir Tamanho do Modelo

Use quantização mais agressiva:
- Q4_0: Boa qualidade, ~1.6GB
- Q3_K_M: Qualidade média, ~1.2GB
- Q2_K: Qualidade básica, ~800MB

### 2. Ajustar Parâmetros de Inferência

Edite `AIModelManager.kt`:

```kotlin
private const val MAX_TOKENS = 256  // Reduzir de 512
private const val TEMPERATURE = 0.8f  // Aumentar para respostas mais criativas
private const val TOP_P = 0.95f  // Aumentar diversidade
```

### 3. Usar GPU (NNAPI)

Para dispositivos com GPU/NPU:

```kotlin
// Em AIModelManager.kt
ctx->params.n_gpu_layers = 20;  // Usar 20 camadas na GPU
```

### 4. Threads de CPU

Ajuste número de threads:

```kotlin
ctx->params.n_threads = 4;  // Aumentar se tiver mais cores
```

### 5. Contexto Menor

Reduza o contexto se tiver pouca RAM:

```kotlin
ctx->params.n_ctx = 1024;  // Reduzir de 2048
```

## 📁 Estrutura do Projeto

```
AuroraEdge/
├── app/
│   ├── src/
│   │   ├── main/
│   │   │   ├── java/com/auroraedge/app/
│   │   │   │   ├── ui/
│   │   │   │   │   └── ChatActivity.kt
│   │   │   │   ├── ai/
│   │   │   │   │   └── AIModelManager.kt
│   │   │   │   ├── model/
│   │   │   │   │   └── ChatMessage.kt
│   │   │   │   └── adapter/
│   │   │   │       └── ChatAdapter.kt
│   │   │   ├── cpp/
│   │   │   │   ├── llama_jni.cpp
│   │   │   │   └── CMakeLists.txt
│   │   │   ├── res/
│   │   │   │   ├── layout/
│   │   │   │   ├── values/
│   │   │   │   └── drawable/
│   │   │   └── AndroidManifest.xml
│   │   └── test/
│   └── build.gradle.kts
├── third_party/
│   └── llama.cpp/
└── README.md
```

## 🐛 Troubleshooting

### Modelo não carrega

- Verifique se o arquivo GGUF está no diretório correto
- Confirme que há espaço suficiente (500MB+)
- Verifique logs: `adb logcat | grep AIModelManager`

### Respostas muito lentas

- Reduza `MAX_TOKENS`
- Use modelo menor (Q3_K_M ou Q2_K)
- Feche outros apps para liberar RAM

### App trava ao iniciar

- Verifique se o dispositivo tem RAM suficiente (2GB+)
- Tente modelo menor
- Reinicie o dispositivo

### Erro de compilação JNI

- Verifique se NDK está instalado
- Confirme versão do NDK (r25c+)
- Limpe build: `./gradlew clean`

## 📝 Licença

Este projeto está sob licença MIT. Veja LICENSE para detalhes.

## 🤝 Contribuindo

Contribuições são bem-vindas! Por favor, abra uma issue ou pull request.

## 📧 Contato

Para dúvidas ou suporte, abra uma issue no repositório.

