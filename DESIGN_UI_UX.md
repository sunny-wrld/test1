# Aurora Edge - Design UI/UX Completo
## Especificação Profissional de Interface

---

## 1. VISÃO GERAL DO DESIGN

### 1.1 Identidade Visual

**Tema**: Futurista, Minimalista, Tecnológico  
**Paleta de Cores**: Azul/Roxo Neon  
**Estilo**: Dark Mode com acentos vibrantes  
**Filosofia**: "Privacidade através da simplicidade"

### 1.2 Princípios de Design

- **Clareza**: Interface intuitiva, sem complexidade desnecessária
- **Performance**: Animações suaves, sem lag
- **Acessibilidade**: Alto contraste, tamanhos de fonte legíveis
- **Consistência**: Padrões visuais uniformes em todas as telas

---

## 2. TELA DE CHAT (PRINCIPAL)

### 2.1 Layout Geral

```
┌─────────────────────────────────────┐
│  [Status Bar - Azul Escuro]        │
├─────────────────────────────────────┤
│  🔒 Aurora Edge    [100% Offline]  │ ← Header Fixo
│  Modelo: Phi-2 Q4_0  [●] Pronto    │
├─────────────────────────────────────┤
│                                     │
│  [Área de Mensagens - Scrollável]  │
│                                     │
│  ┌─────────────┐                   │
│  │ Olá! Como   │ ← Bolha IA        │
│  │ posso ajudar│                   │
│  └─────────────┘                   │
│                                     │
│            ┌─────────────┐         │
│            │ Resuma este │ ← Bolha │
│            │ texto...    │ Usuário │
│            └─────────────┘         │
│                                     │
├─────────────────────────────────────┤
│ [📄] [Input Text]        [Enviar]  │ ← Input Fixo
└─────────────────────────────────────┘
```

### 2.2 Componentes Detalhados

#### Header (Altura: 80dp)

- **Fundo**: `#1E293B` (Surface Dark)
- **Borda Inferior**: 1dp, cor `#475569`
- **Elementos**:
  - **Logo/Ícone**: Aurora Edge (ícone de chip/IA) à esquerda
  - **Status**: Indicador de conexão offline (sempre verde)
  - **Modelo Atual**: Nome do modelo carregado (ex: "Phi-2 Q4_0")
  - **Status do Modelo**: Indicador circular
    - Verde: Pronto
    - Amarelo: Carregando
    - Vermelho: Erro
  - **Menu**: Três pontos (configurações) à direita

#### Área de Mensagens

- **Fundo**: `#0F172A` (Background Dark)
- **Padding**: 16dp nas laterais, 8dp entre mensagens
- **Scroll**: Suave, com snap ao final
- **Indicador de Digitação**: Animação de três pontos pulsantes

#### Bolhas de Mensagem

**Mensagem do Usuário** (Direita):
- **Fundo**: `#6366F1` (Primary - Azul)
- **Texto**: Branco (`#F1F5F9`)
- **Bordas**: 16dp radius, canto inferior direito 4dp
- **Padding**: 12dp horizontal, 10dp vertical
- **Largura Máxima**: 75% da tela
- **Sombra**: Sutil, elevação 2dp

**Mensagem da IA** (Esquerda):
- **Fundo**: `#334155` (Surface Light)
- **Texto**: Branco (`#F1F5F9`)
- **Bordas**: 16dp radius, canto inferior esquerdo 4dp
- **Padding**: 12dp horizontal, 10dp vertical
- **Largura Máxima**: 75% da tela
- **Ícone**: Pequeno chip/IA à esquerda (opcional)

**Mensagem do Sistema** (Centro):
- **Fundo**: `#475569` (Bubble System)
- **Texto**: Cinza claro (`#94A3B8`), itálico
- **Bordas**: 8dp radius (completo)
- **Tamanho de Fonte**: 14sp
- **Padding**: 8dp

#### Input de Texto (Altura: 72dp)

- **Fundo**: `#1E293B` (Surface Dark)
- **Borda Superior**: 1dp, cor `#475569`
- **Layout**: Horizontal, padding 12dp

**Botão Resumir** (Esquerda):
- **Ícone**: 📄 (ícone de documento)
- **Tamanho**: 48dp x 48dp
- **Cor**: `#8B5CF6` (Accent)
- **Estado Desabilitado**: 50% opacidade

**Campo de Texto** (Centro):
- **Fundo**: `#1E293B` (Input Background)
- **Borda**: 1dp, cor `#475569`, radius 12dp
- **Hint**: "Digite sua mensagem..." (cinza)
- **Texto**: Branco, 16sp
- **Padding**: 12dp
- **Máximo de Linhas**: 5 (expansível)
- **IME Options**: Action Send

**Botão Enviar** (Direita):
- **Texto**: "Enviar" ou ícone de seta
- **Cor**: `#6366F1` (Primary)
- **Estado Desabilitado**: Cinza, 50% opacidade
- **Elevação**: 0dp (flat)

### 2.3 Animações

- **Entrada de Mensagem**: Fade in + slide up (300ms)
- **Indicador de Digitação**: Pulsação suave (1s loop)
- **Scroll Automático**: Smooth scroll ao adicionar mensagem
- **Botão Enviar**: Ripple effect ao tocar

### 2.4 Estados Especiais

**Carregando Modelo**:
- Overlay semi-transparente
- Spinner centralizado
- Texto: "Carregando modelo de IA..."
- Progresso: Barra de progresso indeterminada

**Erro**:
- Toast vermelho no topo
- Ícone de alerta
- Mensagem clara do erro

---

## 3. TELA DE CONFIGURAÇÕES

### 3.1 Layout Geral

```
┌─────────────────────────────────────┐
│  ← Configurações                    │ ← Toolbar
├─────────────────────────────────────┤
│                                     │
│  📱 MODELO DE IA                    │
│  ┌─────────────────────────────┐   │
│  │ Phi-2 Q4_0          [>]     │   │ ← Seleção
│  └─────────────────────────────┘   │
│                                     │
│  ⚙️ PARÂMETROS                      │
│  ┌─────────────────────────────┐   │
│  │ Máx. Tokens: 512    [512]   │   │ ← Slider
│  │ Temperatura: 0.7    [0.7]   │   │
│  │ Top-P: 0.9          [0.9]   │   │
│  └─────────────────────────────┘   │
│                                     │
│  🎨 APARÊNCIA                       │
│  ┌─────────────────────────────┐   │
│  │ Tema Escuro        [✓]      │   │ ← Switch
│  │ Fonte: Roboto      [>]      │   │
│  └─────────────────────────────┘   │
│                                     │
│  📊 INFORMAÇÕES                     │
│  ┌─────────────────────────────┐   │
│  │ Versão: 1.0.0               │   │
│  │ Modelo: 1.6 GB              │   │
│  │ RAM Usada: 2.1 GB           │   │
│  └─────────────────────────────┘   │
│                                     │
└─────────────────────────────────────┘
```

### 3.2 Componentes

#### Toolbar
- **Altura**: 56dp
- **Fundo**: `#1E293B`
- **Botão Voltar**: Ícone de seta, cor `#8B5CF6`
- **Título**: "Configurações", 20sp, bold

#### Seções

Cada seção tem:
- **Título**: Ícone + texto, 16sp, bold, cor `#8B5CF6`
- **Card**: Fundo `#1E293B`, radius 12dp, padding 16dp
- **Divisor**: 1dp, cor `#475569`, margin 16dp

#### Seleção de Modelo

- **Lista de Modelos**:
  - Nome do modelo
  - Tamanho (ex: "1.6 GB")
  - Status (Disponível/Carregado)
  - Checkmark se carregado
- **Ação**: Toque abre lista de modelos disponíveis

#### Sliders de Parâmetros

- **Track**: `#475569` (inativo), `#6366F1` (ativo)
- **Thumb**: Círculo, cor `#8B5CF6`, tamanho 24dp
- **Valor**: Exibido à direita, 14sp
- **Range**: 
  - Tokens: 128-1024
  - Temperatura: 0.1-2.0
  - Top-P: 0.1-1.0

#### Switches

- **Track Inativo**: `#475569`
- **Track Ativo**: `#6366F1`
- **Thumb**: Branco, 20dp

#### Informações do Sistema

- **Layout**: Lista de chave-valor
- **Chave**: Cinza (`#94A3B8`), 14sp
- **Valor**: Branco (`#F1F5F9`), 14sp, bold

---

## 4. TELA DE AUTOMAÇÕES (FUTURO)

### 4.1 Layout Geral

```
┌─────────────────────────────────────┐
│  ← Automações                       │
├─────────────────────────────────────┤
│                                     │
│  ➕ Nova Automação                  │ ← FAB
│                                     │
│  📋 AUTOMAÇÕES ATIVAS               │
│  ┌─────────────────────────────┐   │
│  │ 📱 Resumir Notificações     │   │
│  │    Ativo            [ON]    │   │
│  └─────────────────────────────┘   │
│                                     │
│  ┌─────────────────────────────┐   │
│  │ 📧 Responder E-mails        │   │
│  │    Inativo          [OFF]   │   │
│  └─────────────────────────────┘   │
│                                     │
│  📚 TEMPLATES                       │
│  ┌─────────────────────────────┐   │
│  │ Resumir textos              │   │
│  │ Traduzir mensagens          │   │
│  │ Analisar documentos         │   │
│  └─────────────────────────────┘   │
│                                     │
└─────────────────────────────────────┘
```

### 4.2 Componentes

#### FAB (Floating Action Button)
- **Posição**: Canto inferior direito
- **Cor**: `#6366F1` (Primary)
- **Ícone**: ➕ (branco)
- **Elevação**: 6dp
- **Tamanho**: 56dp

#### Cards de Automação

- **Fundo**: `#1E293B`
- **Padding**: 16dp
- **Radius**: 12dp
- **Elementos**:
  - Ícone da automação (esquerda)
  - Nome (centro)
  - Switch (direita)
- **Ação**: Toque abre detalhes/edição

---

## 5. TELA DE LEITURA DE PDF (FUTURO)

### 5.1 Layout Geral

```
┌─────────────────────────────────────┐
│  ← PDF Reader        [⋮]            │
├─────────────────────────────────────┤
│                                     │
│  [Visualizador de PDF]              │
│                                     │
│  ┌─────────────────────────────┐   │
│  │                             │   │
│  │     Conteúdo do PDF         │   │
│  │                             │   │
│  └─────────────────────────────┘   │
│                                     │
├─────────────────────────────────────┤
│ [📄] [Resumir] [Traduzir] [Buscar] │ ← Toolbar
└─────────────────────────────────────┘
```

### 5.2 Componentes

#### Visualizador de PDF
- **Fundo**: Branco (simula papel)
- **Zoom**: Pinch to zoom
- **Scroll**: Vertical e horizontal
- **Páginas**: Indicador de página atual

#### Toolbar Inferior
- **Botões**: Ícones com labels
- **Cor**: `#6366F1`
- **Ações**:
  - Resumir documento
  - Traduzir texto selecionado
  - Buscar no documento
  - Extrair texto

---

## 6. ELEMENTOS VISUAIS GLOBAIS

### 6.1 Tipografia

**Fonte Principal**: Roboto (sistema Android)

- **Títulos**: 20sp, bold, `#F1F5F9`
- **Subtítulos**: 16sp, medium, `#F1F5F9`
- **Corpo**: 16sp, regular, `#F1F5F9`
- **Secundário**: 14sp, regular, `#94A3B8`
- **Pequeno**: 12sp, regular, `#94A3B8`

### 6.2 Ícones

- **Fonte**: Material Icons
- **Tamanho Padrão**: 24dp
- **Cor Padrão**: `#8B5CF6` (Accent)
- **Estados**: 
  - Normal: 100% opacidade
  - Desabilitado: 50% opacidade
  - Hover/Pressed: 80% opacidade

### 6.3 Espaçamento

- **Padding Padrão**: 16dp
- **Margin Padrão**: 8dp
- **Espaçamento entre Elementos**: 12dp
- **Radius Padrão**: 12dp

### 6.4 Elevação e Sombras

- **Cards**: 2dp elevation
- **FAB**: 6dp elevation
- **Dialogs**: 8dp elevation
- **Sombras**: Suaves, cor preta 20% opacidade

### 6.5 Animações Globais

- **Transições de Tela**: Slide horizontal (300ms)
- **Fade In**: 200ms
- **Ripple Effect**: 200ms (Material Design)
- **Loading**: Spinner rotativo (1s loop)

---

## 7. RESPONSIVIDADE

### 7.1 Telas Pequenas (< 360dp)

- Reduzir padding para 12dp
- Largura máxima de bolhas: 85%
- Fonte reduzida em 2sp

### 7.2 Telas Grandes (> 600dp)

- Layout de duas colunas (chat + sidebar)
- Largura máxima: 800dp centralizado
- Aumentar espaçamento

### 7.3 Orientação Landscape

- Chat ocupa largura total
- Input fixo no rodapé
- Header compacto (56dp)

---

## 8. ACESSIBILIDADE

### 8.1 Contraste

- **Texto Primário**: 15:1 (WCAG AAA)
- **Texto Secundário**: 7:1 (WCAG AA)
- **Botões**: 4.5:1 (WCAG AA)

### 8.2 Tamanhos de Toque

- **Mínimo**: 48dp x 48dp
- **Recomendado**: 56dp x 56dp

### 8.3 Suporte a Leitores de Tela

- **Labels**: Todos os elementos têm contentDescription
- **Estados**: Anunciados claramente
- **Navegação**: Lógica e sequencial

---

## 9. PROTÓTIPOS E MOCKUPS

### 9.1 Ferramentas Recomendadas

- **Figma**: Design e prototipagem
- **Adobe XD**: Alternativa
- **Sketch**: macOS

### 9.2 Elementos de Mockup

- **Dispositivo**: Pixel 6 Pro (393x852dp)
- **Status Bar**: Android 12+ (com ícones do sistema)
- **Navigation Bar**: Gestual (sem botões)

---

## 10. IMPLEMENTAÇÃO

### 10.1 Componentes Reutilizáveis

- `AuroraCard`: Card padrão com estilo
- `AuroraButton`: Botão com tema
- `AuroraInput`: Campo de texto estilizado
- `MessageBubble`: Bolha de mensagem
- `LoadingOverlay`: Overlay de carregamento

### 10.2 Temas e Estilos

- `Theme.AuroraEdge`: Tema base
- `Theme.AuroraEdge.Light`: Tema claro (futuro)
- Cores definidas em `colors.xml`
- Estilos em `styles.xml`

---

**Versão**: 1.0.0  
**Designer**: Equipe Aurora Edge  
**Última Atualização**: 2024


