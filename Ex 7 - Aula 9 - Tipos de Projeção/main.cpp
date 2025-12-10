// main.cpp - Cubo 3D + Eixos XYZ com alternância entre Projeção Ortogonal e Perspectiva
// Teclas:
//   O - Projeção Ortogonal
//   P - Projeção Perspectiva
//   ESC - Sair
// Código totalmente comentado para fins didáticos.

#include <GL/glut.h>
#include <cmath>
#include <iostream>

// --------------------------------------------------
// Variável global para armazenar o modo de projeção
// 0 = Ortogonal
// 1 = Perspectiva
// --------------------------------------------------
int projMode = 1; // começa em perspectiva

// Rotação simples do cubo para animar
float ang = 0.0f;

// --------------------------------------------------
// Função que desenha um cubo simples com cores por face
// --------------------------------------------------
void desenhaCubo() {
    glBegin(GL_QUADS);

    // Frente (vermelho)
    glColor3f(1, 0, 0);
    glVertex3f(-1, -1,  1);
    glVertex3f( 1, -1,  1);
    glVertex3f( 1,  1,  1);
    glVertex3f(-1,  1,  1);

    // Trás (verde)
    glColor3f(0, 1, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1,  1, -1);
    glVertex3f( 1,  1, -1);
    glVertex3f( 1, -1, -1);

    // Direita (azul)
    glColor3f(0, 0, 1);
    glVertex3f(1, -1, -1);
    glVertex3f(1,  1, -1);
    glVertex3f(1,  1,  1);
    glVertex3f(1, -1,  1);

    // Esquerda (amarelo)
    glColor3f(1, 1, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1,  1);
    glVertex3f(-1,  1,  1);
    glVertex3f(-1,  1, -1);

    // Topo (magenta)
    glColor3f(1, 0, 1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1,  1);
    glVertex3f( 1, 1,  1);
    glVertex3f( 1, 1, -1);

    // Base (ciano)
    glColor3f(0, 1, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f( 1, -1, -1);
    glVertex3f( 1, -1,  1);
    glVertex3f(-1, -1,  1);

    glEnd();
}

// --------------------------------------------------
// Eixos XYZ para referência visual
// --------------------------------------------------
void desenhaEixos() {
    glLineWidth(3.0f);
    glBegin(GL_LINES);

    // Eixo X (vermelho)
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(3, 0, 0);

    // Eixo Y (verde)
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 3, 0);

    // Eixo Z (azul)
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 3);

    glEnd();
    glLineWidth(1.0f);
}

// --------------------------------------------------
// Função que configura a projeção com base no projMode
// --------------------------------------------------
void atualizaProjecao(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (projMode == 0) {
        // ------------------------------
        // Projeção ORTOGONAL
        // ------------------------------
        float size = 5.0f;
        if (aspect >= 1)
            glOrtho(-size * aspect, size * aspect, -size, size, 1, 50);
        else
            glOrtho(-size, size, -size / aspect, size / aspect, 1, 50);
    }
    else {
        // ------------------------------
        // Projeção PERSPECTIVA
        // ------------------------------
        gluPerspective(60.0, aspect, 1.0, 50.0);
    }

    glMatrixMode(GL_MODELVIEW);
}

// --------------------------------------------------
// reshape callback
// --------------------------------------------------
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    atualizaProjecao(w, h);
}

// --------------------------------------------------
// Display callback
// --------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Posição da câmera
    gluLookAt(6, 5, 8, 0, 0, 0, 0, 1, 0);

    // Desenha eixos
    desenhaEixos();

    // Rotação do cubo
    glRotatef(ang, 1, 1, 0);
    desenhaCubo();

    glutSwapBuffers();
}

// --------------------------------------------------
// Timer para animação
// --------------------------------------------------
void timer(int) {
    ang += 1;
    if (ang >= 360) ang = 0;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// --------------------------------------------------
// Teclado para alternar projeção
// --------------------------------------------------
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'o': case 'O':
            projMode = 0;
            std::cout << "Modo: ORTOGONAL\n";
            atualizaProjecao(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;

        case 'p': case 'P':
            projMode = 1;
            std::cout << "Modo: PERSPECTIVA\n";
            atualizaProjecao(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;

        case 27: // ESC
            exit(0);
    }

    glutPostRedisplay();
}

// --------------------------------------------------
// Inicialização básica
// --------------------------------------------------
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1);
}

// --------------------------------------------------
// main
// --------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cubo + Eixos XYZ — Alternância de Projeção");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}