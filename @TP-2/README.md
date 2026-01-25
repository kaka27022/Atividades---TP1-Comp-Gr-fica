# Visualização 3D de Árvores Arteriais - TP2

## Sobre

Este projeto implementa um visualizador 3D interativo para árvores arteriais a partir de arquivos VTK (Visualization Toolkit). O sistema permite visualizar estruturas arteriais em 3D com modelagem realista usando cilindros, múltiplos modelos de iluminação, transparência e animação de crescimento.

### Funcionalidades Principais

- **Leitura de arquivos VTK 3D (Legacy ASCII)**: Suporte completo para arquivos VTK contendo pontos 3D, linhas e dados de raio
- **Modelagem 3D**: Ramificações modeladas como cilindros com espessura variável baseada nos raios
- **Modos de Raio**: Suporte a raio fixo (todos os ramos com mesmo raio) ou raio variável (cada ramo com seu raio original)
- **Gradiente de Cores**: Visualização com gradiente de cores baseado no raio (azul para raios pequenos, vermelho para raios grandes)
- **Projeção perspectiva**: Visualização 3D com projeção perspectiva realista
- **Câmera orbitante**: Controle interativo da câmera com rotação em torno do modelo e zoom proporcional
- **Modelos de iluminação**: 
  - **Flat Shading**: Iluminação constante por face
  - **Phong Shading**: Iluminação com cálculo de ambiente, difuso e especular
- **Transparência**: Suporte completo a renderização com transparência e blending (alpha = 0.7)
- **Z-Buffer**: Remoção automática de superfícies escondidas
- **Visualização incremental**: Navegação entre diferentes estágios de crescimento da árvore arterial
- **Animação temporal**: Animação automática do crescimento da árvore
- **Preservação de câmera**: A posição da câmera é preservada ao navegar entre arquivos de crescimento

## Como Executar

### Pré-requisitos

- **Compilador C++11**: GCC ou Clang
- **OpenGL e GLUT**: 
  - macOS: OpenGL e GLUT (geralmente já incluídos via frameworks)
  - Linux: `sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev` (Ubuntu/Debian)
  - Windows: Instalar freeglut e configurar as bibliotecas OpenGL

### Compilação

O projeto utiliza um Makefile para facilitar a compilação:

```bash
# Compilar o projeto
make

# Limpar arquivos compilados
make clean

# Limpar e recompilar
make clean && make
```

### Execução

Após compilar, execute o programa fornecendo um arquivo VTK como argumento:

```bash
./tp2_visualizador <arquivo.vtk>
```

**Exemplo:**
```bash
./tp2_visualizador Nterm_128/tree3D_Nterm0128_step0128.vtk
```

O programa detecta automaticamente arquivos de crescimento na mesma série (arquivos com padrão `*_step*.vtk` no mesmo diretório) e ativa o modo de visualização incremental.

### Controles

#### Câmera e Navegação

| Tecla | Ação |
|-------|------|
| **Mouse Arrastar** | Rotacionar câmera (azimuth/elevation) |
| **w** | Zoom in (aproximar - reduz 10% da distância) |
| **S** | Zoom out (afastar - aumenta 10% da distância) |
| **Q / E** | Rotação horizontal (azimuth) - / + |
| **A / D** | Rotação vertical (elevation) - / + |
| **ESPAÇO** | Resetar câmera (distância, ângulos e centro padrão), restaurar todos os segmentos visíveis e limpar seleção |

#### Iluminação e Visualização

| Tecla | Ação |
|-------|------|
| **I** | Alternar modo de iluminação (Flat ↔ Phong) |
| **L** | Toggle iluminação ON/OFF |
| **T** | Toggle transparência (ON/OFF - alpha = 0.7) |
| **R** | Alternar modo de raio (Fixo ↔ Variável) |
| **ESC** | Sair do programa |

#### Visualização Incremental e Animação

| Tecla | Ação |
|-------|------|
| **[** | Arquivo anterior na série de crescimento |
| **]** | Próximo arquivo na série de crescimento |
| **PageUp** | Aumentar quantidade de segmentos visíveis (~5% por vez) |
| **PageDown** | Diminuir quantidade de segmentos visíveis (~5% por vez) |
| **M** | Toggle animação automática do crescimento |

### Informações na Tela

O programa exibe na parte superior da tela:
- Estado da iluminação (ON/OFF e modo: Flat/Phong)
- Modo de raio (FIXO/VARIÁVEL)
- Estado da transparência (ON/OFF)
- Número de segmentos visíveis (atual/total)
- Parâmetros da câmera (distância, azimuth, elevação)
- Informações de crescimento (arquivo atual/total, quando aplicável)
- Estado da animação (quando ativada)

## Detalhes de Implementação

### Arquitetura do Projeto

O código está organizado em módulos separados para facilitar manutenção e compreensão:

```
src/
├── main.cpp          # Ponto de entrada e inicialização GLUT
├── globals.h/cpp     # Variáveis globais e estruturas de dados 3D
├── utils.h/cpp       # Funções auxiliares (leitura VTK 3D, cálculo vetorial)
├── interface.h/cpp   # Funções de renderização (cilindros, iluminação, desenho)
└── handlers.h/cpp    # Handlers de eventos (teclado, mouse)
```

### Estruturas de Dados

- **`Point3D`**: Representa um ponto 3D com coordenadas (x, y, z) e operações vetoriais (normalização, produto escalar, produto vetorial)
- **`Line3D`**: Representa um segmento conectando dois pontos 3D, com raio associado e índices dos pontos
- **`Camera`**: Estrutura para câmera orbitante com controle de distância, azimuth e elevation
- **`Light`**: Configuração de iluminação com propriedades ambiente, difusa e especular

### Modelagem 3D - Cilindros

Os ramos arteriais são modelados como cilindros 3D conectando os pontos. Cada cilindro:
- Tem raio proporcional ao raio do segmento no arquivo VTK (modo variável) ou raio fixo médio (modo fixo)
- É composto por múltiplos segmentos (configurável via `cylinder_quality`, padrão: 16)
- Possui bases (tampas) nas extremidades
- É renderizado com normais calculadas para iluminação adequada
- Aplica gradiente de cores baseado no raio original (mesmo no modo fixo)

### Modos de Raio

#### Modo Variável (padrão)
- Cada segmento mantém seu raio original do arquivo VTK
- Preserva as diferenças de espessura entre ramos (tronco mais grosso, ramos finos)
- Escala: raio máximo do tronco em ~2.5% do tamanho da cena; limites entre 0.15% e 4% do `data_scale`

#### Modo Fixo
- Todos os segmentos usam o mesmo raio (raio médio calculado dos dados)
- Uniformiza a espessura de todos os ramos
- Útil para visualizar a estrutura geométrica sem variações de espessura
- Usa a mesma escala do modo variável (referência 2.5%, limites 0.15%–4%)

### Gradiente de Cores

O gradiente de cores é aplicado com base no raio normalizado de cada segmento (0 a 1):
- **Azul** (raios pequenos) → **Ciano** → **Verde** → **Amarelo** → **Laranja** → **Vermelho** (raios grandes)

O gradiente é calculado mesmo no modo fixo (usa o raio original para a cor), mas todos os segmentos terão o mesmo tamanho visual.

### Modelos de Iluminação

#### Flat Shading
- Calcula uma única cor por face do cilindro
- Utiliza o produto escalar entre a normal da face e a direção da luz
- Mais rápido, mas menos realista
- A cor base (do gradiente) é preservada e combinada com a iluminação

#### Phong Shading
- Calcula a cor em cada vértice considerando:
  - **Componente ambiente**: Iluminação base (30% da cor base)
  - **Componente difusa**: Produto escalar entre normal e direção da luz (90% da cor base)
  - **Componente especular**: Reflexo especular (Phong) baseado no produto escalar entre direção de reflexão e direção de visualização (branco para reflexos)
- Mais realista, com reflexos especulares destacados
- Mantém o gradiente de cores original combinado com a iluminação

### Projeção Perspectiva

A projeção perspectiva é implementada usando `gluPerspective()` com:
- Campo de visão (FOV): 45 graus
- Aspect ratio: Calculado automaticamente da janela
- Near plane: 0.1 unidades
- Far plane: 1000 unidades

### Câmera Orbitante

A câmera orbita em torno do centro do modelo usando coordenadas esféricas:
- **Distance**: Distância da câmera ao centro (controle de zoom proporcional - 10% por tecla)
- **Azimuth**: Ângulo horizontal (rotação ao redor do eixo Y) em graus
- **Elevation**: Ângulo vertical (inclinação da câmera) em graus, limitado entre -89° e +89°

A posição do olho (eye) é calculada como:
```
eye.x = center.x + distance * cos(elevation) * cos(azimuth)
eye.y = center.y + distance * sin(elevation)
eye.z = center.z + distance * cos(elevation) * sin(azimuth)
```

**Importante**: A posição da câmera é preservada ao navegar entre arquivos de crescimento usando as teclas `[` e `]`, permitindo manter o mesmo ponto de vista durante a navegação. A tecla ESPAÇO reseta a câmera para distância 10, azimuth 45°, elevação 30° e centro (0,0,0), além de restaurar todos os segmentos visíveis e limpar a seleção.

### Transparência e Z-Buffer

- **Transparência**: Implementada usando blending OpenGL com `GL_BLEND` e `GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA`
  - Alpha padrão: 0.7 (70% de opacidade)
  - Usa `glColor4f` com canal alpha para todos os vértices quando habilitada
  - `glDepthMask(GL_FALSE)` é usado durante renderização transparente para evitar problemas com Z-buffer
- **Z-Buffer**: Habilitado com `GL_DEPTH_TEST` e `GL_LEQUAL` para remoção automática de superfícies escondidas

### Animação Temporal

A animação do crescimento permite visualizar o desenvolvimento progressivo da árvore arterial:
- Navega automaticamente entre os arquivos de crescimento
- Controlável via tecla M (toggle)
- Velocidade ajustável via `animation_speed` (padrão: 1.0)
- Timer baseado em `glutTimerFunc` para atualização a ~20 FPS

### Leitura de Arquivos VTK 3D

O parser VTK suporta o formato Legacy ASCII, lendo:
- **POINTS**: Coordenadas dos vértices (x, y, z) como floats
- **LINES**: Conectividade entre pontos, criando segmentos consecutivos
- **CELL_DATA / POINT_DATA**: Dados escalares de raio associados aos segmentos

O parser também:
- Calcula automaticamente o bounding box dos dados
- Ajusta a câmera inicial para focar no centro do modelo
- Calcula a distância inicial da câmera proporcional ao tamanho dos dados
- Detecta automaticamente arquivos de crescimento na mesma série

### Cálculo de Normais

As normais são calculadas para cada face do cilindro:
- Para a superfície lateral: Normal perpendicular à direção do segmento, apontando para fora
- Para as bases: Normal apontando para fora do cilindro (paralela à direção do segmento)
- As normais são normalizadas antes de serem usadas na iluminação

### Visualização Incremental

O sistema suporta visualização incremental de duas formas:

1. **Por arquivos**: Navegação manual entre diferentes estágios de crescimento usando `[` e `]`
2. **Por segmentos**: Controle gradual da quantidade de segmentos visíveis usando PageUp/PageDown (~5% por vez)

Ambas preservam a posição da câmera durante a navegação.

### Compatibilidade

O código é compatível com:
- **macOS**: Utiliza frameworks nativos (`-framework OpenGL -framework GLUT`)
- **Linux**: Utiliza bibliotecas padrão (`-lGL -lGLU -lglut`)
- **Windows**: Requer configuração adicional das bibliotecas OpenGL

O sistema detecta automaticamente o sistema operacional durante a compilação via `__APPLE__` e ajusta as flags e bibliotecas apropriadamente.

## Requisitos do TP2

### Implementados ✓

- ✓ Leitura e representação de dados 3D (coordenadas x,y,z, conectividade e raios) de arquivos VTK Legacy ASCII
- ✓ Modelagem de ramos como cilindros 3D com espessura proporcional aos raios
- ✓ Modo de raio fixo (todos os ramos com mesmo raio) e variável (cada ramo com seu raio original)
- ✓ Gradiente de cores baseado no raio (azul → vermelho)
- ✓ Projeção perspectiva usando `gluPerspective`
- ✓ Controle de câmera orbitante com coordenadas esféricas
- ✓ Dois modelos de iluminação (Flat e Phong) funcionando corretamente
- ✓ Transparência funcional com blending e alpha channel
- ✓ Remoção de superfícies escondidas (Z-buffer) com `GL_DEPTH_TEST`
- ✓ Interação 3D completa (câmera, zoom, iluminação, transparência, modo de raio)
- ✓ Animação temporal do crescimento da árvore arterial
- ✓ Navegação entre arquivos de crescimento preservando a câmera
- ✓ Visualização incremental de segmentos (PageUp/PageDown)

### Observações Técnicas

- O cálculo das normais é feito para cada face do cilindro
- A iluminação Phong implementa componentes ambiente (30%), difusa (90%) e especular (reflexos brancos)
- A transparência usa alpha = 0.7 e `glDepthMask(GL_FALSE)` durante renderização transparente
- O Z-buffer garante que objetos mais próximos sejam renderizados sobre os mais distantes
- O zoom é proporcional (10% de mudança por tecla W/S) para melhor controle
- A câmera é preservada ao navegar entre arquivos para manter a mesma perspectiva
- Os raios são escalados automaticamente: o raio máximo do tronco é referência em ~2,5% do tamanho da cena (`data_scale`), com limites entre 0,15% e 4%, mantendo as proporções entre modos fixo e variável

## Notas de Desenvolvimento

- A implementação usa OpenGL 2.x (fixed-function pipeline) para compatibilidade
- O código está estruturado de forma modular para facilitar manutenção e extensão
- Mensagens de debug podem ser habilitadas removendo comentários no código dos handlers
