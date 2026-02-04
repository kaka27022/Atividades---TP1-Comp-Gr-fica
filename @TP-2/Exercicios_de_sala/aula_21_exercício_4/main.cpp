#include <GL/freeglut.h>
#include <iostream>
#include "cube_renderer.h"

// Variáveis globais para controle (Slide 16)
bool cullingEnabled = false;
GLenum frontFaceMode = GL_CCW; 
float rotX = 0.0f, rotY = 0.0f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Posicionamento da câmera e rotação
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // Requisito 1: Ativação/Desativação do Culling
    if (cullingEnabled) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);

    // Configuração padrão: Descartar a face de trás (Back)
    glCullFace(GL_BACK);

    // Requisito 2: Alternar definição de face frontal (CCW ou CW)
    glFrontFace(frontFaceMode);

    desenhaCubo();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'c': case 'C': // Liga/Desliga Culling
            cullingEnabled = !cullingEnabled;
            std::cout << "[INFO] Culling: " << (cullingEnabled ? "LIGADO" : "DESLIGADO") << std::endl;
            break;
        case 'f': case 'F': // Alterna FrontFace
            frontFaceMode = (frontFaceMode == GL_CCW) ? GL_CW : GL_CCW;
            std::cout << "[INFO] FrontFace definida como: " << (frontFaceMode == GL_CCW ? "CCW" : "CW") << std::endl;
            break;
        case 27: exit(0); break; // Tecla ESC
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP)    rotX -= 5.0f;
    if (key == GLUT_KEY_DOWN)  rotX += 5.0f;
    if (key == GLUT_KEY_LEFT)  rotY -= 5.0f;
    if (key == GLUT_KEY_RIGHT) rotY += 5.0f;
    glutPostRedisplay();
}

void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST); // Z-buffer ativo para correta profundidade
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.33, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Exercicio UFOP - Back-Face Culling");
    
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    std::cout << "--- COMANDOS ---" << std::endl;
    std::cout << "C: Alternar Culling (Enable/Disable)" << std::endl;
    std::cout << "F: Alternar FrontFace (CCW/CW)" << std::endl;
    std::cout << "Setas: Rotacionar o cubo" << std::endl;
    
    glutMainLoop();
    return 0;
}