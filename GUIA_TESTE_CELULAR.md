# 📱 Guia Completo: Como Testar Aurora Edge no Seu Celular

Este guia te ajudará a instalar e testar o Aurora Edge no seu dispositivo Android.

---

## 📋 Pré-requisitos

### No Computador
- ✅ **Android Studio** (Hedgehog ou superior)
- ✅ **JDK 17** ou superior
- ✅ **Android SDK** instalado
- ✅ **NDK r25c** ou superior (para compilar código nativo)
- ✅ **ADB** (Android Debug Bridge) - vem com Android Studio

### No Celular
- ✅ **Android 7.0** (API 24) ou superior
- ✅ **Mínimo 2GB RAM** (recomendado 4GB+)
- ✅ **Pelo menos 3GB de espaço livre** (para app + modelo)
- ✅ **Modo Desenvolvedor ativado**
- ✅ **Depuração USB ativada**

---

## 🔧 Passo 1: Ativar Modo Desenvolvedor no Celular

1. Abra **Configurações** no seu celular
2. Vá em **Sobre o telefone** (ou **Sobre o dispositivo**)
3. Encontre **Número da versão** (ou **Versão MIUI**, etc)
4. Toque **7 vezes** no número da versão
5. Digite seu PIN/senha quando solicitado
6. Aparecerá: **"Agora você é um desenvolvedor!"**

### Ativar Depuração USB

1. Volte para **Configurações**
2. Procure **Opções do desenvolvedor** (ou **Opções de desenvolvedor**)
3. Ative **Depuração USB**
4. Aceite o aviso de segurança

---

## 📲 Passo 2: Conectar Celular ao Computador

1. Conecte o celular ao PC via cabo USB
2. No celular, quando aparecer popup, toque em **"Permitir depuração USB"**
3. Marque **"Sempre permitir deste computador"**
4. Toque em **OK**

### Verificar Conexão

Abra o **Terminal** (PowerShell no Windows) e execute:

```bash
adb devices
```

Você deve ver algo como:
```
List of devices attached
ABC123XYZ    device
```

Se aparecer **"unauthorized"**, volte ao celular e aceite a permissão.

---

## 🏗️ Passo 3: Compilar o App no Android Studio

### 3.1 Abrir Projeto

1. Abra **Android Studio**
2. Clique em **File > Open**
3. Navegue até a pasta `AuroraEdge`
4. Clique em **OK**
5. Aguarde o Gradle sincronizar (pode demorar alguns minutos na primeira vez)

### 3.2 Verificar Configuração

1. Vá em **File > Project Structure**
2. Verifique:
   - **Compile SDK Version**: 34
   - **Min SDK Version**: 24
   - **Target SDK Version**: 34
   - **NDK Version**: r25c ou superior

### 3.3 Compilar APK

**Opção A: Via Android Studio (Recomendado)**

1. Conecte seu celular via USB
2. Clique no menu dropdown ao lado do botão **Run** (▶️)
3. Selecione seu dispositivo
4. Clique em **Run** (▶️) ou pressione **Shift + F10**
5. Aguarde compilação e instalação automática

**Opção B: Via Linha de Comando**

```bash
# No PowerShell (Windows) ou Terminal (Linux/Mac)
cd C:\Users\asmag\OneDrive\Documents\vs\AuroraEdge

# Compilar APK de debug
.\gradlew assembleDebug

# O APK estará em:
# app/build/outputs/apk/debug/app-debug.apk
```

---

## 🤖 Passo 4: Baixar um Modelo GGUF

O app precisa de um modelo de IA quantizado em formato GGUF. Você pode baixar de vários lugares:

### Opção 1: Hugging Face (Recomendado)

**Modelos Recomendados para Celular:**

#### Phi-2 Q4_0 (~1.6GB) - Melhor para começar
```bash
# Link direto:
https://huggingface.co/microsoft/phi-2-gguf/resolve/main/phi-2.Q4_0.gguf
```

#### Gemma 2B Q4_0 (~1.4GB) - Boa qualidade
```bash
https://huggingface.co/bartowski/gemma-2b-it-GGUF/resolve/main/gemma-2b-it-Q4_0.gguf
```

#### Mistral 7B Q4_0 (~4GB) - Alta qualidade (requer mais RAM)
```bash
https://huggingface.co/TheBloke/Mistral-7B-Instruct-v0.2-GGUF/resolve/main/mistral-7b-instruct-v0.2.Q4_0.gguf
```

### Como Baixar

**Método 1: Navegador**
1. Abra o link no navegador
2. O download começará automaticamente
3. Salve o arquivo com nome `phi-2-q4_0.gguf` (ou outro nome)

**Método 2: wget (Windows)**
```powershell
# Instale wget se não tiver (vem com Git Bash ou baixe separadamente)
wget https://huggingface.co/microsoft/phi-2-gguf/resolve/main/phi-2.Q4_0.gguf -O phi-2-q4_0.gguf
```

**Método 3: curl**
```powershell
curl -L https://huggingface.co/microsoft/phi-2-gguf/resolve/main/phi-2.Q4_0.gguf -o phi-2-q4_0.gguf
```

---

## 📂 Passo 5: Copiar Modelo para o Celular

### Via ADB (Recomendado)

```bash
# 1. Primeiro, criar diretório de modelos no celular
adb shell mkdir -p /sdcard/Android/data/com.auroraedge.app/files/models

# 2. Copiar modelo para o celular
# (Substitua pelo caminho real do seu arquivo)
adb push C:\caminho\para\phi-2-q4_0.gguf /sdcard/Android/data/com.auroraedge.app/files/models/phi-2-q4_0.gguf

# Exemplo real:
# adb push C:\Users\asmag\Downloads\phi-2-q4_0.gguf /sdcard/Android/data/com.auroraedge.app/files/models/phi-2-q4_0.gguf
```

**Nota:** O caminho interno real do app é:
```
/data/data/com.auroraedge.app/files/models/
```

Mas usando `/sdcard/Android/data/...` funciona porque o Android cria um link simbólico.

### Via Transferência Manual (Alternativa)

1. Conecte celular ao PC via USB
2. Selecione **Transferência de arquivos** (MTP)
3. Navegue até: `Celular > Android > data > com.auroraedge.app > files`
4. Crie pasta `models` se não existir
5. Copie o arquivo `.gguf` para a pasta `models`

### Via App de Arquivos no Celular

1. Baixe um app de gerenciador de arquivos (ex: **Files by Google**)
2. Copie o modelo GGUF para a pasta do celular (Downloads)
3. Mova manualmente para:
   ```
   Android > data > com.auroraedge.app > files > models
   ```

---

## ✅ Passo 6: Testar o App Offline

### 6.1 Preparar Teste Offline

1. **Desconecte a internet** do celular:
   - Ative **Modo Avião** OU
   - Desative Wi-Fi e Dados móveis

2. Abra o app **Aurora Edge** no celular

### 6.2 Primeiro Uso

1. Ao abrir o app, você verá uma tela de carregamento
2. O app tentará carregar o modelo (pode demorar **30-60 segundos**)
3. Aguarde até aparecer: **"Modelo pronto"** ou **"Pronto para usar"**

### 6.3 Testar Chat

1. Digite uma mensagem: **"Olá, como você está?"**
2. Toque no botão **Enviar** (ou pressione Enter)
3. Aguarde alguns segundos (primeira resposta pode demorar 5-15 segundos)
4. Você deve ver a resposta da IA gerada localmente!

### 6.4 Testar Função de Resumo

1. Toque no botão **📄** (resumir texto)
2. Cole um texto longo (pode copiar de um site/documento)
3. Clique em **Resumir**
4. Aguarde o resumo ser gerado

---

## 🔍 Verificar Logs e Debug

Se algo não funcionar, você pode ver os logs:

```bash
# Ver todos os logs do app
adb logcat | findstr "AIModelManager"

# Ou mais específico:
adb logcat | findstr "AuroraEdge"
```

### Logs Importantes

- ✅ **"Biblioteca nativa carregada"** - JNI está OK
- ✅ **"Carregando modelo: ..."** - Tentando carregar modelo
- ✅ **"Modelo carregado com sucesso"** - Modelo está pronto
- ❌ **"Modelo não encontrado"** - Arquivo GGUF não está no lugar certo
- ❌ **"Falha ao carregar modelo"** - Problema com arquivo ou memória

---

## 🐛 Problemas Comuns e Soluções

### ❌ "Modelo não encontrado"

**Solução:**
```bash
# Verificar se arquivo existe no celular
adb shell ls -la /sdcard/Android/data/com.auroraedge.app/files/models/

# Se não existir, copie novamente
adb push phi-2-q4_0.gguf /sdcard/Android/data/com.auroraedge.app/files/models/
```

### ❌ App trava ao carregar modelo

**Possíveis causas:**
- Modelo muito grande para a RAM disponível
- Arquivo GGUF corrompido

**Soluções:**
1. Use modelo menor (Phi-2 Q4_0 ou Gemma 2B)
2. Feche outros apps para liberar RAM
3. Reinicie o celular
4. Baixe o modelo novamente

### ❌ "Erro ao carregar biblioteca nativa"

**Solução:**
- O código nativo (JNI) não foi compilado corretamente
- Verifique se NDK está instalado no Android Studio
- Recompile o projeto: `Build > Clean Project` e depois `Build > Rebuild Project`

### ❌ Respostas muito lentas

**Soluções:**
1. Use modelo menor (menos parâmetros)
2. Reduza `MAX_TOKENS` no código (edite `AIModelManager.kt`)
3. Feche apps em background
4. Use dispositivo com mais RAM/CPU

### ❌ ADB não reconhece dispositivo

**Soluções:**
1. Instale drivers USB do fabricante do celular
2. Tente outro cabo USB
3. Desative e reative **Depuração USB**
4. No Windows: Instale **Google USB Driver** via Android Studio SDK Manager

---

## 📊 Verificar Uso de Recursos

### Ver uso de RAM

```bash
adb shell dumpsys meminfo com.auroraedge.app
```

### Ver uso de CPU

```bash
adb shell top | findstr "auroraedge"
```

---

## 🎯 Checklist Final

Antes de testar, verifique:

- [ ] Celular tem modo desenvolvedor ativado
- [ ] Depuração USB está ativada
- [ ] ADB reconhece o dispositivo (`adb devices`)
- [ ] App está instalado no celular
- [ ] Modelo GGUF foi copiado para a pasta correta
- [ ] Celular tem pelo menos 2GB RAM livre
- [ ] Modo Avião está ATIVADO (para testar offline)

---

## 📝 Notas Importantes

1. **Primeira execução é sempre mais lenta** - O modelo precisa ser carregado na memória
2. **Modelos grandes (>4GB) podem não funcionar** em dispositivos básicos
3. **Qualidade vs. Velocidade** - Modelos menores são mais rápidos, mas podem ter qualidade inferior
4. **Bateria** - IA local consome bastante bateria, mantenha o celular carregado durante testes

---

## 🚀 Próximos Passos

Após testar com sucesso:

1. Experimente diferentes modelos
2. Ajuste parâmetros (temperatura, max_tokens)
3. Teste em diferentes dispositivos
4. Reporte bugs ou melhorias

---

## 📞 Suporte

Se encontrar problemas:

1. Verifique os logs: `adb logcat`
2. Confirme que seguiu todos os passos
3. Teste com modelo diferente
4. Verifique requisitos mínimos do dispositivo

---

**Boa sorte com os testes! 🎉**


