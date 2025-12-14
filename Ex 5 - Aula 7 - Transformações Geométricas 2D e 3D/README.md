# Exercícios de Transformações Geométricas 3D

Este programa implementa os exercícios de revisão sobre transformações geométricas 3D.

## Problemas Implementados

### Problema 1 - Revisão Algébrica
Rotaciona um objeto definido pelos pontos A(1,0,0), B(0,1,0), C(0,0,1) em torno do eixo X por 180°.

**Resultado esperado:**
- A'(1,0,0)
- B'(0,-1,0)  
- C'(0,0,-1)

### Problema 2 - Composição de Transformações
Demonstra a sequência correta de chamadas OpenGL para realizar:
1. Rotação de 45° em torno do eixo Y local
2. Translação de 10 unidades no eixo Z
3. Escala por fator 2.0

**Sequência correta:**
```cpp
glScaled(2.0, 2.0, 2.0);
glTranslatef(0.0, 0.0, 10.0);
glRotatef(45.0, 0, 1, 0);
```

## Como Compilar e Executar

### Usando g++ diretamente:
```bash
g++ -o transformacoes_3d transformacoes_3d.cpp -lm
./transformacoes_3d
```

### Usando Makefile:
```bash
make
make run
```

### Para limpar arquivos compilados:
```bash
make clean
```

## Estrutura do Código

- **Ponto3D**: Estrutura para representar pontos em 3D
- **Matriz4x4**: Classe para matrizes de transformação homogêneas 4x4
- **problema1()**: Implementação do Problema 1
- **problema2()**: Implementação do Problema 2

## Funcionalidades

- Matrizes de rotação em X e Y
- Matriz de translação
- Matriz de escala
- Composição de transformações
- Aplicação de transformações a pontos
- Visualização de matrizes e resultados

## Saída do Programa

O programa mostra:
1. Os pontos originais e transformados para o Problema 1
2. As matrizes de transformação individuais e composta para o Problema 2
3. Verificação passo-a-passo das transformações
4. Comparação entre transformação composta e aplicação sequencial
