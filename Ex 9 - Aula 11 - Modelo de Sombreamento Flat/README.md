# Comparação GL_FLAT vs GL_SMOOTH - Modelo de Sombreamento

Este projeto implementa um exercício prático de Computação Gráfica que demonstra as diferenças entre os modelos de sombreamento **GL_FLAT** e **GL_SMOOTH** em OpenGL.

## Objetivos

Comparar visualmente os efeitos dos modelos de sombreamento:

- **GL_FLAT**: Sombreamento uniforme por face (Flat Shading)
- **GL_SMOOTH**: Sombreamento suave com interpolação entre vértices (Gouraud Shading)

## Funcionalidades Implementadas

1. **Iluminação Habilitada**: `glEnable(GL_LIGHTING)` com fonte de luz `GL_LIGHT0`
2. **Material Colorido**: `glEnable(GL_COLOR_MATERIAL)` para usar `glColor` como material
3. **Callback de Teclado**: Alternância entre modos de sombreamento
4. **Objetos 3D**: Cubo colorido com faces multicoloridas e esfera
5. **Controles Interativos**: Rotação e alternância de objetos

## Controles

| Tecla    | Função                      |
| -------- | --------------------------- |
| **F**    | Ativa modo GL_FLAT          |
| **G**    | Ativa modo GL_SMOOTH        |
| **C**    | Alterna entre Cubo e Esfera |
| **↑↓←→** | Rotaciona o objeto          |
| **ESC**  | Sair do programa            |

## Compilação e Execução

### macOS

```bash
# Compilar
make

# Executar
make run

# Ou executar diretamente
./shading_comparison
```

### Linux (Ubuntu/Debian)

```bash
# Instalar dependências
sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev

# Editar Makefile: descomentar a linha LIBS para Linux
# LIBS = -lGL -lGLU -lglut

# Compilar e executar
make
make run
```

## Diferenças Observáveis

### GL_FLAT (Flat Shading)

- **Características**:
  - Cada face tem cor uniforme
  - Bordas bem definidas entre faces
  - Aparência mais "facetada"
  - Processamento mais rápido

### GL_SMOOTH (Gouraud Shading)

- **Características**:
  - Transições suaves entre cores
  - Bordas menos definidas
  - Aparência mais realística
  - Interpolação de cores entre vértices

## Desafio Conceitual

**Por que o brilho especular desaparece ou se comporta de forma estranha no modo GL_SMOOTH (Gouraud) em comparação com GL_FLAT?**

### Resposta:

O comportamento diferente do brilho especular entre GL_FLAT e GL_SMOOTH está relacionado a **quando** e **onde** o cálculo de iluminação é realizado:

#### GL_FLAT:

- O cálculo de iluminação é feito **uma vez por face**
- O brilho especular é calculado no centro da face
- Se a luz estiver posicionada de forma que o ângulo de reflexão especular seja ideal para uma face, todo o polígono será brilhante

#### GL_SMOOTH (Gouraud):

- O cálculo de iluminação é feito **nos vértices**
- As cores são **interpoladas linearmente** entre os vértices
- **Problema**: A reflexão especular é **não-linear** e depende do ângulo de visão
- A interpolação linear pode "perder" picos de brilho especular que ocorrem no meio das faces
- O brilho especular pode desaparecer quando interpolado entre vértices que não recebem reflexão especular direta

#### Solução Moderna:

Para reflexões especulares mais precisas, usa-se **Phong Shading** (per-pixel lighting), onde:

- Os cálculos são feitos **por pixel** no fragment shader
- As **normais** são interpoladas (não as cores finais)
- O cálculo de iluminação é feito com a normal interpolada em cada pixel

## Estrutura do Projeto

```
Modelo de Sombreamento Flat/
├── main.cpp          # Código principal
├── Makefile          # Arquivo de compilação
└── README.md         # Este arquivo
```

## Dependências

- **OpenGL**: Biblioteca gráfica 3D
- **GLUT**: OpenGL Utility Toolkit para janelas e eventos
- **Compilador C++**: g++ com suporte a C++11

## Conceitos Demonstrados

1. **Modelos de Sombreamento**: Flat vs Gouraud
2. **Sistema de Iluminação**: Luz ambiente, difusa e especular
3. **Materiais**: Uso de cores como propriedades do material
4. **Geometria 3D**: Cubos e esferas com normais corretas
5. **Interação**: Callbacks de teclado e mouse
