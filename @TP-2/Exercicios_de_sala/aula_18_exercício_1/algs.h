#ifndef ALGS_H
#define ALGS_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

// Estrutura simples para Ponto 2D
struct Point {
    float x, y;
};

// Estrutura para Aresta (Ponto de referência e Normal)
// Baseado no Slide 21 [cite: 183]
struct Edge {
    Point pE; // Ponto na aresta
    Point N;  // Normal apontando para DENTRO
};

// Protótipo da função Cyrus-Beck
// Retorna true se a linha for visível, preenchendo outP0 e outP1
bool cyrusBeckClip(Point p0, Point p1, const std::vector<Edge>& edges, Point &outP0, Point &outP1);

#endif