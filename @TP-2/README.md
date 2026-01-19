# Visualização 3D de Árvores Arteriais - TP2

## Sobre

Este projeto implementa um visualizador 3D interativo para árvores arteriais a partir de arquivos VTK (Visualization Toolkit). O sistema permite visualizar estruturas arteriais em 3D com modelagem realista usando cilindros, múltiplos modelos de iluminação, transparência e animação de crescimento.

### Funcionalidades Principais

- **Leitura de arquivos VTK 3D (Legacy ASCII)**: Suporte completo para arquivos VTK contendo pontos 3D, linhas e dados de raio
- **Modelagem 3D**: Ramificações modeladas como cilindros com espessura variável baseada nos raios
- **Projeção perspectiva**: Visualização 3D com projeção perspectiva realista
- **Câmera orbitante**: Controle interativo da câmera com rotação em torno do modelo
- **Modelos de iluminação**: 
  - **Flat Shading**: Iluminação constante por face
  - **Phong Shading**: Iluminação com cálculo de ambiente, difuso e especular
- **Transparência**: Suporte a renderização com transparência e blending
- **Z-Buffer**: Remoção automática de superfícies escondidas
- **Visualização incremental**: Navegação entre diferentes estágios de crescimento da árvore arterial
- **Animação temporal**: Animação automática do crescimento da árvore
- **Interação 3D**: Seleção de segmentos para visualização de propriedades

## Como Executar

### Pré-requisitos

- **Compilador C++11**: GCC ou Clang
- **OpenGL e GLUT**: 
  - macOS: OpenGL e GLUT (geralmente já incluídos)
  - Linux: `sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev` (Ubuntu/Debian)
  - Windows: Instalar freeglut e configurar as bibliotecas OpenGL

### Compilação

O projeto utiliza um Makefile para facilitar a compilação:

```bash
# Compilar o projeto
make build

# Limpar arquivos compilados
make clean

# Limpar e recompilar
make rebuild

# Compilar e executar com exemplo
make run
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

| Tecla | Ação |
|-------|------|
| **Mouse Arrastar** | Rotacionar câmera (azimuth/elevation) |
| **W / S** | Zoom in / out |
| **Q / E** | Rotação horizontal (azimuth) -/ + |
| **A / D** | Rotação vertical (elevation) -/ + |
| **I** | Alternar modo de iluminação (Flat/Phong) |
| **L** | Toggle iluminação ON/OFF |
| **T** | Toggle transparência |
| **[ / ]** | Arquivo anterior / próximo de crescimento |
| **PageUp / PageDown** | Incrementar / decrementar segmentos visíveis |
| **M** | Toggle animação do crescimento |
| **ESPAÇO** | Resetar câmera |
| **ESC** | Sair do programa |

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

- **`Point3D`**: Representa um ponto 3D com coordenadas (x, y, z) e operações vetoriais
- **`Line3D`**: Representa um segmento conectando dois pontos 3D, com raio associado
- **`Camera`**: Estrutura para câmera orbitante com controle de distância, azimuth e elevation
- **`Light`**: Configuração de iluminação com propriedades ambiente, difusa e especular

### Modelagem 3D - Cilindros

Os ramos arteriais são modelados como cilindros 3D conectando os pontos. Cada cilindro:
- Tem raio proporcional ao raio do segmento no arquivo VTK
- É composto por múltiplos segmentos (configurável via `cylinder_quality`)
- Possui bases (tampas) nas extremidades
- É renderizado com normais calculadas para iluminação adequada

### Modelos de Iluminação

#### Flat Shading
- Calcula uma única cor por face do cilindro
- Utiliza o produto escalar entre a normal da face e a direção da luz
- Mais rápido, mas menos realista

#### Phong Shading
- Calcula a cor em cada vértice considerando:
  - **Componente ambiente**: Iluminação base
  - **Componente difusa**: Produto escalar entre normal e direção da luz
  - **Componente especular**: Reflexo especular (Phong) baseado no produto escalar entre direção de reflexão e direção de visualização
- Mais realista, mas computacionalmente mais caro

### Projeção Perspectiva

A projeção perspectiva é implementada usando `gluPerspective()` com:
- Campo de visão (FOV): 45 graus
- Aspect ratio: Calculado automaticamente da janela
- Near plane: 0.1 unidades
- Far plane: 1000 unidades

### Câmera Orbitante

A câmera orbita em torno do centro do modelo usando coordenadas esféricas:
- **Distance**: Distância da câmera ao centro
- **Azimuth**: Ângulo horizontal (rotação ao redor do eixo Y)
- **Elevation**: Ângulo vertical (inclinação da câmera)

A posição do olho (eye) é calculada como:
```
eye.x = center.x + distance * cos(elevation) * cos(azimuth)
eye.y = center.y + distance * sin(elevation)
eye.z = center.z + distance * cos(elevation) * sin(azimuth)
```

### Transparência e Z-Buffer

- **Transparência**: Implementada usando blending OpenGL com `GL_BLEND` e `GL_SRC_ALPHA`
- **Z-Buffer**: Habilitado com `GL_DEPTH_TEST` para remoção automática de superfícies escondidas

### Animação Temporal

A animação do crescimento permite visualizar o desenvolvimento progressivo da árvore arterial:
- Navega automaticamente entre os arquivos de crescimento
- Controlável via tecla M
- Velocidade ajustável via `animation_speed`

### Leitura de Arquivos VTK 3D

O parser VTK suporta o formato Legacy ASCII, lendo:
- **POINTS**: Coordenadas dos vértices (x, y, z)
- **LINES**: Conectividade entre pontos, criando segmentos consecutivos
- **CELL_DATA / POINT_DATA**: Dados escalares de raio associados aos segmentos

### Cálculo de Normais

As normais são calculadas para cada face do cilindro:
- Para a superfície lateral: Normal perpendicular à direção do segmento
- Para as bases: Normal apontando para fora do cilindro
- As normais são normalizadas antes de serem usadas na iluminação

### Compatibilidade

O código é compatível com:
- **macOS**: Utiliza frameworks nativos (OpenGL, GLUT)
- **Linux**: Utiliza bibliotecas padrão (libGL, libGLU, libglut)
- **Windows**: Requer configuração adicional das bibliotecas OpenGL

O sistema detecta automaticamente o sistema operacional durante a compilação e ajusta as flags e bibliotecas apropriadamente.

## Requisitos do TP2

### Implementados ✓

- ✓ Leitura e representação de dados 3D (coordenadas, conectividade e raios)
- ✓ Modelagem de ramos como cilindros 3D
- ✓ Projeção perspectiva
- ✓ Controle de câmera orbitante
- ✓ Dois modelos de iluminação (Flat e Phong)
- ✓ Transparência
- ✓ Remoção de superfícies escondidas (Z-buffer)
- ✓ Interação 3D (seleção de segmentos - parcialmente implementado)
- ✓ Animação temporal do crescimento da árvore arterial

### Observações

- O cálculo das normais é feito para cada face do cilindro
- A iluminação Phong implementa componentes ambiente, difusa e especular
- A transparência pode ser habilitada/desabilitada em tempo real
- O Z-buffer garante que objetos mais próximos sejam renderizados sobre os mais distantes
