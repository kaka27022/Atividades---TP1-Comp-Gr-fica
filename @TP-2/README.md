# Visualização Iterativa de Árvores Arteriais

## Sobre

Este projeto implementa um visualizador 2D interativo para árvores arteriais a partir de arquivos VTK (Visualization Toolkit). O sistema permite visualizar o crescimento incremental de estruturas arteriais através de diferentes estágios de desenvolvimento, além de oferecer ferramentas de transformação geométrica e recorte de segmentos.

### Funcionalidades Principais

- **Leitura de arquivos VTK (Legacy ASCII)**: Suporte completo para arquivos VTK contendo pontos, linhas e dados de raio
- **Transformações geométricas 2D**: Translação, rotação e escala aplicadas através de matrizes homogêneas
- **Projeção ortográfica**: Visualização 2D com preservação de aspect ratio
- **Visualização incremental do crescimento**: Navegação entre diferentes estágios de crescimento da árvore arterial
- **Recorte de segmentos (Cohen-Sutherland)**: Algoritmo de clipping para visualização de regiões específicas
- **Visualização incremental de segmentos**: Controle sobre quantos segmentos são renderizados dentro de um arquivo

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
./tp1_visualizador <arquivo.vtk>
```

**Exemplo:**
```bash
./tp1_visualizador Nterm_064/tree2D_Nterm0064_step0064.vtk
```

O programa detecta automaticamente arquivos de crescimento na mesma série (arquivos com padrão `*_step*.vtk` no mesmo diretório) e ativa o modo de visualização incremental.

### Controles

| Tecla | Ação |
|-------|------|
| **Setas (← → ↑ ↓)** | Translação da visualização |
| **r / R** | Rotação horária / anti-horária (5° por vez) |
| **+ / -** | Aumentar / diminuir escala |
| **c** | Ativar/desativar recorte (clipping) |
| **x / z** | Aumentar / diminuir tamanho da janela de recorte |
| **[ / ]** | Arquivo anterior / próximo de crescimento |
| **PageUp / PageDown** | Incrementar / decrementar segmentos visíveis |
| **ESPAÇO** | Resetar todas as transformações |
| **ESC** | Sair do programa |

## Detalhes de Implementação

### Arquitetura do Projeto

O código está organizado em módulos separados para facilitar manutenção e compreensão:

```
src/
├── main.cpp          # Ponto de entrada e inicialização GLUT
├── globals.h/cpp     # Variáveis globais e estruturas de dados
├── utils.h/cpp       # Funções auxiliares (leitura VTK, manipulação de arquivos)
├── interface.h/cpp    # Funções de interface gráfica (desenho, transformações, clipping)
└── handlers.h/cpp    # Handlers de eventos (teclado)
```

### Estruturas de Dados

- **`Point2D`**: Representa um ponto 2D com coordenadas (x, y)
- **`Line`**: Representa um segmento conectando dois pontos, com raio associado
- **`OutCode`**: Enumeração para o algoritmo de recorte Cohen-Sutherland

### Transformações Geométricas

As transformações são aplicadas na ordem: **Escala → Rotação → Translação**, utilizando matrizes homogêneas 3x3. A função `applyTransform()` aplica todas as transformações a cada ponto antes da renderização.

### Algoritmo de Recorte Cohen-Sutherland

O algoritmo de recorte implementado utiliza códigos de região (outcodes) para determinar rapidamente se um segmento está completamente dentro, completamente fora, ou parcialmente dentro da janela de recorte. A janela de recorte é calculada dinamicamente como uma fração da viewport atual, centralizada na tela.

### Leitura de Arquivos VTK

O parser VTK suporta o formato Legacy ASCII, lendo:
- **POINTS**: Coordenadas dos vértices (x, y, z) - apenas x e y são utilizados
- **LINES**: Conectividade entre pontos, criando segmentos consecutivos
- **CELL_DATA / POINT_DATA**: Dados escalares de raio associados aos segmentos

### Visualização Incremental

O sistema oferece dois níveis de visualização incremental:

1. **Entre arquivos**: Navegação entre diferentes estágios de crescimento (ex: step0008, step0016, etc.)
2. **Dentro do arquivo**: Controle sobre quantos segmentos são renderizados, permitindo visualizar o crescimento gradual dentro de um único arquivo

### Projeção Ortográfica

A projeção 2D mantém o aspect ratio da janela, ajustando automaticamente os limites da viewport para evitar distorções. A função `reshape()` recalcula a projeção sempre que a janela é redimensionada.

### Variáveis Globais

As variáveis globais são organizadas em categorias:
- Dados da árvore (pontos, linhas, raios)
- Parâmetros de transformação (translação, rotação, escala)
- Estado de visualização incremental
- Configurações de recorte
- Dimensões da janela

### Compatibilidade

O código é compatível com:
- **macOS**: Utiliza frameworks nativos (OpenGL, GLUT)
- **Linux**: Utiliza bibliotecas padrão (libGL, libGLU, libglut)
- **Windows**: Requer configuração adicional das bibliotecas OpenGL

O sistema detecta automaticamente o sistema operacional durante a compilação e ajusta as flags e bibliotecas apropriadamente.

