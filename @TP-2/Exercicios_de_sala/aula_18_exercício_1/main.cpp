#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "algs.h"

// Dimensões da Janela
const int WIDTH = 800;
const int HEIGHT = 600;

// Estado Global
std::vector<Edge> clipPolygon; 
std::vector<Point> polygonVertices; 
struct LineSegment { Point p0, p1; };
std::vector<LineSegment> lines; 

// Variáveis de interação
bool isDrawing = false;
Point tempP0;
Point tempMouse;

// Configura o polígono (Janela de Recorte)
void setupClipWindow() {
    // Hexágono Convexo
    std::vector<Point> verts = {
        {300, 150}, {500, 150}, {600, 300}, 
        {500, 450}, {300, 450}, {200, 300}
    };
    polygonVertices = verts;
    clipPolygon.clear(); // Garante que está vazio ao iniciar

    // Calcula normais para dentro
    for (size_t i = 0; i < verts.size(); i++) {
        Point p1 = verts[i];
        Point p2 = verts[(i + 1) % verts.size()];

        Edge e;
        e.pE = p1; 
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        e.N = { -dy, dx }; // Normal (rotação 90 graus para esquerda)
        clipPolygon.push_back(e);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Configura a matriz de modelagem (Onde as coisas estão)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 1. Desenhar a Janela de Recorte (Polígono Azul)
    glColor3f(0.0, 0.0, 1.0); 
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    for (auto v : polygonVertices) glVertex2f(v.x, v.y);
    glEnd();

    // 2. Processar e Desenhar as Linhas
    for (const auto& line : lines) {
        // A. Linha original (Cinza claro)
        glColor3f(0.8, 0.8, 0.8); 
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex2f(line.p0.x, line.p0.y);
        glVertex2f(line.p1.x, line.p1.y);
        glEnd();

        // B. Recorte (Vermelho)
        Point cP0, cP1;
        if (cyrusBeckClip(line.p0, line.p1, clipPolygon, cP0, cP1)) {
            glColor3f(1.0, 0.0, 0.0);
            glLineWidth(3.0);
            glBegin(GL_LINES);
            glVertex2f(cP0.x, cP0.y);
            glVertex2f(cP1.x, cP1.y);
            glEnd();
        }
    }

    // Linha elástica (enquanto desenha)
    if (isDrawing) {
        glColor3f(0.6, 0.6, 0.6);
        glLineWidth(1.0);
        glEnable(GL_LINE_STIPPLE); // Linha tracejada
        glLineStipple(1, 0x00FF);
        glBegin(GL_LINES);
        glVertex2f(tempP0.x, tempP0.y);
        glVertex2f(tempMouse.x, tempMouse.y);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }

    glutSwapBuffers(); // IMPORTANTE: Troca os buffers (GLUT_DOUBLE)
}

// Garante que a coordenada (0,0) a (800,600) se mantenha
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); // Muda para a lente da câmera
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT); // Define o sistema de coordenadas 2D
    glMatrixMode(GL_MODELVIEW); // Volta para o modo de desenho
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDrawing = true;
            tempP0 = { (float)x, (float)(HEIGHT - y) }; // Inverte Y do mouse
            tempMouse = tempP0;
        } else if (state == GLUT_UP && isDrawing) {
            isDrawing = false;
            Point p1 = { (float)x, (float)(HEIGHT - y) };
            lines.push_back({ tempP0, p1 });
            glutPostRedisplay();
        }
    }
}

void motion(int x, int y) {
    if (isDrawing) {
        tempMouse = { (float)x, (float)(HEIGHT - y) };
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fundo Branco
    setupClipWindow();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // IMPORTANTE: GLUT_DOUBLE evita piscadas e telas brancas
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Atividade Cyrus-Beck - UFOP");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape); // Nova função de segurança
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    std::cout << "--- Sistema de Recorte Cyrus-Beck ---\n";
    std::cout << "1. O Hexagono Azul e a janela de recorte.\n";
    std::cout << "2. Clique e arraste para criar linhas.\n";
    std::cout << "3. Linha Cinza = Original | Vermelha = Recortada.\n";

    glutMainLoop();
    return 0;
}