# Implementação de Gouraud Shading - Normais por Vértice

Este projeto implementa o cálculo de normais por vértice para habilitar o Gouraud Shading em objetos 3D, conforme especificado na atividade de Computação Gráfica.

## Objetivos Implementados

**Passo 1**: Carregamento/Definição de Malha - Implementadas malhas simples (cubo e octaedro)  
**Passo 2**: Cálculo de Normais de Face - Via produto vetorial e normalização  
**Passo 3**: Cálculo de Normais de Vértice - Soma das normais de faces adjacentes  
**Passo 4**: Normalização - Normalização dos vetores resultantes  
**Passo 5**: Renderização - Uso de `glNormal3fv()` antes de `glVertex3fv()`  
**Teste**: Comparação entre `GL_SMOOTH` (Gouraud) e `GL_FLAT` (Flat Shading)

## Estrutura do Projeto

```
├── main.cpp          # Aplicação principal OpenGL
├── mesh.h            # Definição da classe Mesh
├── mesh.cpp          # Implementação dos algoritmos de cálculo de normais
├── vector3d.h        # Classe para operações com vetores 3D
├── CMakeLists.txt    # Configuração de compilação
└── README.md         # Este arquivo
```

## Compilação

### macOS

```bash
mkdir build && cd build
cmake ..
make
./gouraud_shading
```

### Linux

```bash
# Instalar dependências (Ubuntu/Debian)
sudo apt-get install freeglut3-dev libgl1-mesa-dev

mkdir build && cd build
cmake ..
make
./gouraud_shading
```

## Como Usar

### Controles

- **S**: Alternar entre Gouraud Shading (GL_SMOOTH) e Flat Shading (GL_FLAT)
- **M**: Alternar entre Cubo e Octaedro
- **Mouse**: Arrastar para rotacionar o objeto
- **ESC**: Sair da aplicação

### Observação dos Resultados

- **Gouraud Shading (GL_SMOOTH)**: Suavização aparente nas transições entre faces
- **Flat Shading (GL_FLAT)**: Bordas definidas entre as faces

## Algoritmos Implementados

### 1. Cálculo de Normais de Face

```cpp
Vector3D edge1 = v2 - v1;
Vector3D edge2 = v3 - v1;
Vector3D normal = edge1.cross(edge2).normalize();
```

### 2. Cálculo de Normais de Vértice

```cpp
// Para cada face, somar sua normal aos vértices que a compõem
for (const auto& face : faces) {
    vertexNormals[face.v1] = vertexNormals[face.v1] + face.normal;
    vertexNormals[face.v2] = vertexNormals[face.v2] + face.normal;
    vertexNormals[face.v3] = vertexNormals[face.v3] + face.normal;
}

// Normalizar cada normal de vértice
for (auto& normal : vertexNormals) {
    normal = normal.normalize();
}
```

### 3. Renderização com Normais por Vértice

```cpp
glNormal3f(vertexNormals[face.v1].x, vertexNormals[face.v1].y, vertexNormals[face.v1].z);
glVertex3f(vertices[face.v1].x, vertices[face.v1].y, vertices[face.v1].z);
```

## Malhas Implementadas

### Cubo

- 8 vértices
- 12 faces triangulares
- Demonstra bem a diferença entre os dois modelos de sombreamento

### Octaedro

- 6 vértices
- 8 faces triangulares
- Forma mais suave, ideal para visualizar o efeito do Gouraud Shading

## Diferenças Visuais

A principal diferença observável entre os dois modelos:

- **GL_FLAT**: Cada face tem cor uniforme, criando bordas visíveis
- **GL_SMOOTH**: Interpolação suave entre vértices, criando gradientes

O efeito é mais pronunciado em objetos com poucas faces, como os implementados neste projeto.

## Tecnologias Utilizadas

- **C++17**: Linguagem de programação
- **OpenGL**: API gráfica para renderização
- **GLUT**: Biblioteca para janelas e eventos
- **CMake**: Sistema de build multiplataforma
