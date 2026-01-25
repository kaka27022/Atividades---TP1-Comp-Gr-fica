/*Exercıcio de Implementacao: Sutherland-Hodgman
Objetivo: Implementar a funcao clipPolygon que recebe um polıgono generico e o recorta
contra uma janela retangular 2D.
O Desafio
1.Utilize a estrutura std::vector<Point>paragerenciarosvertices.
2.Implemente o loop de recorte para as 4 bordas (esquerda,direita, topo, fundo).
3.No display(),desenhe o poligono original em wireframe(vermelho) e opolıgono
recortado preenchido(verde). */

#include <GL/glut.h>
#include <vector>
#include <iostream>

// 1. Definições de Estruturas e Enums (Devem vir primeiro)
struct Point {
    float x, y;
};

typedef std::vector<Point> Polygon;

enum Edge { LEFT, RIGHT, BOTTOM, TOP };

// 2. Variáveis Globais (Limites da Janela de Recorte)
float xMin = 150.0, xMax = 350.0, yMin = 150.0, yMax = 350.0;
Polygon originalPoly;

// 3. Funções Auxiliares de Recorte
bool isInside(Point p, int edge, float clipVal) {
    switch (edge) {
        case LEFT:   return p.x >= clipVal;
        case RIGHT:  return p.x <= clipVal;
        case BOTTOM: return p.y >= clipVal;
        case TOP:    return p.y <= clipVal;
    }
    return false;
}

Point getIntersection(Point p1, Point p2, int edge, float clipVal) {
    Point res;
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    
    if (edge == LEFT || edge == RIGHT) {
        res.x = clipVal;
        res.y = p1.y + dy * (clipVal - p1.x) / dx;
    } else { // TOP ou BOTTOM
        res.y = clipVal;
        if (dx == 0) res.x = p1.x; // Evita divisão por zero em linhas verticais
        else res.x = p1.x + dx * (clipVal - p1.y) / dy;
    }
    return res;
}

// 4. Lógica do Algoritmo Sutherland-Hodgman
Polygon clip(Polygon poly, int edge, float clipVal) {
    Polygon newPoly;
    if (poly.empty()) return newPoly;

    for (size_t i = 0; i < poly.size(); i++) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % poly.size()];

        bool in1 = isInside(p1, edge, clipVal);
        bool in2 = isInside(p2, edge, clipVal);

        if (in1 && in2) {
            newPoly.push_back(p2);
        } else if (in1 && !in2) {
            newPoly.push_back(getIntersection(p1, p2, edge, clipVal));
        } else if (!in1 && in2) {
            newPoly.push_back(getIntersection(p1, p2, edge, clipVal));
            newPoly.push_back(p2);
        }
    }
    return newPoly;
}

// 5. Função de Desenho
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenhar a Janela de Recorte (Cinza para referência)
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xMin, yMin); glVertex2f(xMax, yMin);
        glVertex2f(xMax, yMax); glVertex2f(xMin, yMax);
    glEnd();

    // 1. Desenhar Original (Vermelho Wireframe)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        for(auto p : originalPoly) glVertex2f(p.x, p.y);
    glEnd();

    // 2. Executar Recorte (Encadeado)
    Polygon clipped = originalPoly;
    clipped = clip(clipped, LEFT, xMin);
    clipped = clip(clipped, RIGHT, xMax);
    clipped = clip(clipped, BOTTOM, yMin);
    clipped = clip(clipped, TOP, yMax);

    // 3. Desenhar Recortado (Verde Preenchido)
    if (!clipped.empty()) {
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_POLYGON);
            for(auto p : clipped) glVertex2f(p.x, p.y);
        glEnd();
    }

    glFlush();
}

// 6. Main
int main(int argc, char** argv) {
    // Definindo um polígono que sai para fora da janela para testar o recorte
    originalPoly = {{100, 100}, {400, 200}, {250, 400}}; 

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Sutherland-Hodgman Clipping");
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500); 

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}