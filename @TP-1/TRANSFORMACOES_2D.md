# Transformações Geométricas 2D - Fundamentos Matemáticos

## Introdução

Este documento explica a implementação matemática das transformações geométricas 2D utilizadas no visualizador de árvores arteriais. As transformações permitem manipular a visualização através de translação, rotação e escala.

## Coordenadas Homogêneas

Para facilitar a composição de transformações, utilizamos **coordenadas homogêneas**. Um ponto 2D `(x, y)` é representado como `(x, y, 1)` em coordenadas homogêneas.

### Vantagens das Coordenadas Homogêneas

1. **Unificação**: Translação, rotação e escala podem ser representadas como multiplicação de matrizes
2. **Composição**: Múltiplas transformações podem ser combinadas em uma única matriz
3. **Eficiência**: Aplicar uma matriz composta é mais eficiente que aplicar transformações sequenciais

## Transformações Fundamentais

### 1. Translação

A translação move um ponto por um deslocamento `(tx, ty)`.

**Matriz de Translação:**
```
T = [1  0  tx]
    [0  1  ty]
    [0  0  1 ]
```

**Aplicação:**
```
[x']   [1  0  tx] [x]
[y'] = [0  1  ty] [y]
[1 ]   [0  0  1 ] [1]
```

**Resultado:**
- `x' = x + tx`
- `y' = y + ty`

### 2. Rotação

A rotação gira um ponto em torno da origem por um ângulo θ (em radianos).

**Matriz de Rotação:**
```
R = [cos(θ)  -sin(θ)  0]
    [sin(θ)   cos(θ)  0]
    [0        0       1]
```

**Aplicação:**
```
[x']   [cos(θ)  -sin(θ)  0] [x]
[y'] = [sin(θ)   cos(θ)  0] [y]
[1 ]   [0        0       1] [1]
```

**Resultado:**
- `x' = x·cos(θ) - y·sin(θ)`
- `y' = x·sin(θ) + y·cos(θ)`

**Nota:** No código, o ângulo é armazenado em graus e convertido para radianos:
```cpp
float rad = angle * M_PI / 180.0f;
```

### 3. Escala

A escala multiplica as coordenadas por fatores `sx` e `sy`.

**Matriz de Escala:**
```
S = [sx  0   0]
    [0   sy  0]
    [0   0   1]
```

**Aplicação:**
```
[x']   [sx  0   0] [x]
[y'] = [0   sy  0] [y]
[1 ]   [0   0   1] [1]
```

**Resultado:**
- `x' = sx · x`
- `y' = sy · y`

**Nota:** No código, utilizamos escala uniforme (`sx = sy = scale`).

## Composição de Transformações

### Ordem Importante

A ordem das transformações é **crucial**. A multiplicação de matrizes não é comutativa:
- `T · R · S ≠ S · R · T`

### Transformação Composta no Projeto

No código, a ordem de aplicação é: **Escala → Rotação → Translação**

**Matriz Composta:**
```
M = T · R · S
```

**Expansão:**
```
M = [1  0  tx] [cos(θ)  -sin(θ)  0] [scale  0       0]
    [0  1  ty] [sin(θ)   cos(θ)  0] [0      scale  0]
    [0  0  1 ] [0        0       1] [0      0       1]
```

**Resultado após multiplicação:**
```
M = [scale·cos(θ)  -scale·sin(θ)  tx]
    [scale·sin(θ)   scale·cos(θ)   ty]
    [0              0              1 ]
```

### Por que esta ordem?

1. **Escala primeiro**: Aplica o fator de escala aos pontos originais
2. **Rotação depois**: Rotaciona os pontos já escalados em torno da origem
3. **Translação por último**: Move o objeto rotacionado e escalado para a posição final

Esta ordem garante que:
- A rotação ocorre em torno da origem (0, 0)
- A escala é aplicada antes da rotação
- A translação move o objeto já transformado

## Implementação no Código

### Função `applyTransform()`

```cpp
void applyTransform(Point2D& p) {
    // Matriz de transformação: T * R * S
    // Ordem: Escala -> Rotação -> Translação
    
    float rad = angle * M_PI / 180.0f;
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    
    // Aplicar escala
    float sx = p.x * scale;
    float sy = p.y * scale;
    
    // Aplicar rotação
    float rx = sx * cos_a - sy * sin_a;
    float ry = sx * sin_a + sy * cos_a;
    
    // Aplicar translação
    p.x = rx + tx;
    p.y = ry + ty;
}
```

### Análise Passo a Passo

1. **Conversão de ângulo:**
   ```cpp
   float rad = angle * M_PI / 180.0f;
   ```
   Converte graus para radianos (necessário para funções trigonométricas).

2. **Cálculo de seno e cosseno:**
   ```cpp
   float cos_a = cos(rad);
   float sin_a = sin(rad);
   ```
   Pré-calcula valores trigonométricos (otimização).

3. **Aplicação de escala:**
   ```cpp
   float sx = p.x * scale;
   float sy = p.y * scale;
   ```
   Equivale a: `S · P`

4. **Aplicação de rotação:**
   ```cpp
   float rx = sx * cos_a - sy * sin_a;
   float ry = sx * sin_a + sy * cos_a;
   ```
   Equivale a: `R · (S · P)`
   
   **Matematicamente:**
   - `rx = sx·cos(θ) - sy·sin(θ)`
   - `ry = sx·sin(θ) + sy·cos(θ)`

5. **Aplicação de translação:**
   ```cpp
   p.x = rx + tx;
   p.y = ry + ty;
   ```
   Equivale a: `T · (R · (S · P))`

## Exemplo Numérico

Considere um ponto `P = (2, 3)` e as transformações:
- **Escala:** `scale = 2.0`
- **Rotação:** `angle = 45°` (π/4 radianos)
- **Translação:** `tx = 1.0`, `ty = 2.0`

### Passo 1: Escala
```
sx = 2 · 2 = 4
sy = 2 · 3 = 6
```

### Passo 2: Rotação (45°)
```
cos(45°) = √2/2 ≈ 0.707
sin(45°) = √2/2 ≈ 0.707

rx = 4 · 0.707 - 6 · 0.707 = -1.414
ry = 4 · 0.707 + 6 · 0.707 = 7.071
```

### Passo 3: Translação
```
x' = -1.414 + 1.0 = -0.414
y' = 7.071 + 2.0 = 9.071
```

**Resultado final:** `P' = (-0.414, 9.071)`

## Propriedades Matemáticas

### 1. Associatividade

A composição de transformações é associativa:
```
(T · R) · S = T · (R · S)
```

### 2. Matriz Inversa

Para reverter uma transformação, calculamos a matriz inversa:
```
M⁻¹ = (T · R · S)⁻¹ = S⁻¹ · R⁻¹ · T⁻¹
```

**Inversas individuais:**
- **Translação:** `T⁻¹(tx, ty) = T(-tx, -ty)`
- **Rotação:** `R⁻¹(θ) = R(-θ)`
- **Escala:** `S⁻¹(s) = S(1/s)`

### 3. Preservação de Retas

Transformações afins preservam:
- Retas paralelas permanecem paralelas
- Proporções ao longo de retas são mantidas
- Combinações convexas são preservadas

## Otimizações Implementadas

1. **Cálculo direto:** Em vez de construir e multiplicar matrizes 3x3, aplicamos as transformações sequencialmente (mais eficiente computacionalmente).

2. **Pré-cálculo trigonométrico:** Seno e cosseno são calculados uma vez por frame, não por ponto.

3. **Transformação in-place:** O ponto é modificado diretamente, evitando cópias desnecessárias.

## Referências Matemáticas

- **Geometria Afim:** As transformações implementadas são transformações afins, que preservam retas e paralelismo.

- **Grupo de Transformações:** O conjunto de transformações afins forma um grupo sob composição.

- **Álgebra Linear:** A representação matricial permite aplicar conceitos de álgebra linear para análise e otimização.

## Conclusão

A implementação utiliza coordenadas homogêneas e composição de transformações para permitir manipulação eficiente e intuitiva da visualização. A ordem Escala → Rotação → Translação garante comportamento previsível e desejável para visualização interativa.

