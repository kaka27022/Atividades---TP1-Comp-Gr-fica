// main.c - Quadrado verde com rotacao e mudanca de cor de fundo via teclado
#include <GL/gl.h>
#include <GL/glut.h>

// Variavel global para guardar a cor do fundo
float bgColor[3] = {0.2f, 0.2f, 0.2f}; // cor inicial: cinza escuro

// Funcao que desenha o quadrado
void display(void) {
    // Define cor de funco
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Define cor do quadrado (verde)
    glColor3f(0.0f, 1.0f, 0.0f);

    // Define a matriz de modelagem
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Aplica uma rotacao de 30 no eixo Z
    glRotatef(30.0f, 0.0f,0.0f, 1.0f);

    // Desenha o quadrado centrado na origem
    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
    glEnd();

    glutSwapBuffers();
}

// Funcao de callback para redimencionar a janela
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // coordenadas simples 2D
    glMatrixMode(GL_MODELVIEW);
}

// Callback de teclado
void keyboard(unsigned char key, int x, int y) {
    (void)x; (void)y; // evita warnings

    if (key == 27) { // tecla ESC
        exit(0);
    }

    // Se pressionar r muda a cor de fundo pra vermelho
    if (key == 'r' || key == 'R') {
        bgColor[0] = 1.0f;
        bgColor[1] = 0.0f;
        bgColor[2] = 0.0f;
        glutPostRedisplay(); // forca redeseno
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Quadrado com rotacao e fundo interativo");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
// ------------------Compilacao----------------------------------

// gcc main.c -o quadrado -lGL -lGLU -lglut
// ./quadrado