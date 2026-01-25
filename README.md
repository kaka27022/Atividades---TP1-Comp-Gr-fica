# Visualização Interativa e Realista de Árvores Arteriais

**Universidade Federal de Ouro Preto (UFOP)**
**DECOM / ICEB**
**BCC327 – Computação Gráfica**

Trabalho Prático **TP1 e TP2**
**Tema:** Visualização Interativa e Realista de Árvores Arteriais Geradas pelo Método CCO

---

## 📌 Contexto e Motivação

O método **CCO (Constrained Constructive Optimization)** é amplamente utilizado para a geração de modelos computacionais de **árvores arteriais** em duas e três dimensões, a partir de princípios **fisiológicos**, **geométricos** e **topológicos**. Esses modelos são fundamentais para estudos em áreas como biomecânica, bioengenharia e visualização científica.

Os dados das árvores arteriais são fornecidos no formato **VTK (Visualization Toolkit)**, contendo informações como:

* Geometria dos segmentos (posição dos vértices);
* Topologia (conectividade entre os ramos);
* Morfometria (raio dos segmentos arteriais).

O objetivo deste trabalho é desenvolver um **sistema de visualização interativa e realista** desses modelos arteriais, utilizando bibliotecas gráficas como **OpenGL/GLUT**, permitindo a exploração visual das estruturas geradas pelo método CCO.

O trabalho é dividido em duas etapas:

* **TP1:** Visualização de modelos **2D**;
* **TP2:** Visualização de modelos **3D**.

---

## 🎯 Objetivos

### Objetivo Geral

Desenvolver um visualizador gráfico capaz de carregar, interpretar e exibir modelos de árvores arteriais em 2D e 3D a partir de arquivos VTK.

### Objetivos Específicos

* Ler arquivos VTK no formato *legacy ASCII*;
* Representar árvores arteriais com segmentos cilíndricos (tubos);
* Permitir visualização com **raio fixo** e **raio variável** dos segmentos;
* Implementar interações básicas de visualização (zoom, rotação, translação);
* Diferenciar TP1 (2D) e TP2 (3D) conforme os requisitos do problema.

---

## 🧩 Descrição do Trabalho

### 🔹 TP1 – Visualização 2D

No TP1, o sistema deve ser capaz de visualizar árvores arteriais **bidimensionais**, representando os segmentos como tubos em 2D. Os modelos possuem diferentes números de segmentos terminais (por exemplo, 64 e 256).

Funcionalidades esperadas:

* Leitura de arquivos VTK 2D;
* Renderização de segmentos com:

  * raio fixo;
  * raio variável;
* Visualização clara da estrutura topológica da árvore.

### 🔹 TP2 – Visualização 3D

No TP2, a visualização é estendida para **modelos tridimensionais**, aumentando a complexidade geométrica e visual. Os modelos podem conter 128, 256 ou até 512 segmentos terminais.

Funcionalidades esperadas:

* Leitura de arquivos VTK 3D;
* Renderização de árvores arteriais em 3D;
* Representação dos segmentos como tubos cilíndricos:

  * com raio fixo;
  * com raio variável;
* Interação com a cena (rotação, zoom e navegação);
* Melhor percepção de profundidade e estrutura espacial.

---

## 🛠️ Tecnologias Utilizadas

* **Linguagem:** C++
* **Bibliotecas Gráficas:**

  * OpenGL
  * GLUT / FreeGLUT
* **Formato de dados:** VTK (legacy ASCII)
* **Ambiente:** Linux / macOS / Windows (MSYS2)

---

## ▶️ Compilação e Execução

O projeto utiliza um **Makefile** para facilitar a compilação.

### Compilar o projeto:

```bash
make
```

### Executar com arquivo de exemplo:

```bash
make run
```

### Limpar arquivos compilados:

```bash
make clean
```

### Limpar e recompilar:

```bash
make rebuild
```
---

## 👩‍💻👨‍💻 Autoria

Trabalho desenvolvido como parte da disciplina **BCC327 – Computação Gráfica**, do curso de Ciência da Computação da **Universidade Federal de Ouro Preto (UFOP)**.

[Maria Clara](https://github.com/kaka27022)

[Niege Reis](https://github.com/niegereis)

[Lourrane Lindsay](https://github.com/Lou-LAE)

[Eduardo Silva](https://github.com/EduardoGPS-18)

[Camila Aparecida](https://github.com/millow-w/)

---

## 📌 Observações Finais

Este projeto reforça conceitos fundamentais de computação gráfica, como:

* pipeline gráfico;
* transformações geométricas;
* visualização científica;
* renderização interativa de estruturas complexas.

Além disso, destaca a aplicação prática da computação gráfica em problemas reais da área biomédica.


---




