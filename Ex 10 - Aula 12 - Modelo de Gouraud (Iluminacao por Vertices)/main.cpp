#include <iostream>
#include <string>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "mesh.h"

// Variáveis globais
Mesh currentMesh;
bool useGouraudShading = true;
bool useCube = true;
float rotationX = 0.0f;
float rotationY = 0.0f;
int lastMouseX = 0;
int lastMouseY = 0;
bool mousePressed = false;

// Configurações da janela
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void setupLighting() {
    // Habilitar iluminação
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Configurar luz ambiente
    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    
    // Configurar luz difusa
    GLfloat diffuseLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    
    // Configurar luz especular
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    // Posição da luz
    GLfloat lightPosition[] = {2.0f, 2.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // Material
    GLfloat materialAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat materialDiffuse[] = {0.7f, 0.7f, 0.9f, 1.0f};
    GLfloat materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialShininess[] = {50.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
}

void loadMesh() {
    if (useCube) {
        currentMesh = Mesh::createCube();
    } else {
        currentMesh = Mesh::createOctahedron();
    }
    
    // Calcular normais
    currentMesh.calculateFaceNormals();
    currentMesh.calculateVertexNormals();
    
    std::cout << "Malha carregada: " << (useCube ? "Cubo" : "Octaedro") << std::endl;
    std::cout << "Vértices: " << currentMesh.getVertices().size() << std::endl;
    std::cout << "Faces: " << currentMesh.getFaces().size() << std::endl;
}

void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    setupLighting();
    loadMesh();
    
    // Configurar modelo de sombreamento inicial
    glShadeModel(useGouraudShading ? GL_SMOOTH : GL_FLAT);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Posicionar câmera
    gluLookAt(0.0, 0.0, 5.0,  // posição da câmera
              0.0, 0.0, 0.0,  // ponto de foco
              0.0, 1.0, 0.0); // vetor up
    
    // Aplicar rotações
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    
    // Renderizar a malha
    currentMesh.render(useGouraudShading);
    
    // Renderizar texto informativo
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Texto informativo
    std::string modeText = useGouraudShading ? "Gouraud Shading (GL_SMOOTH)" : "Flat Shading (GL_FLAT)";
    std::string shapeText = useCube ? "Cubo" : "Octaedro";
    
    glRasterPos2f(10, WINDOW_HEIGHT - 20);
    for (char c : modeText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    
    glRasterPos2f(10, WINDOW_HEIGHT - 45);
    for (char c : shapeText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    glRasterPos2f(10, 40);
    std::string controls = "Controles: S = Trocar Shading, M = Trocar Malha, Mouse = Rotacionar";
    for (char c : controls) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHTING);
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 's':
        case 'S':
            useGouraudShading = !useGouraudShading;
            glShadeModel(useGouraudShading ? GL_SMOOTH : GL_FLAT);
            std::cout << "Modo de sombreamento: " << 
                (useGouraudShading ? "Gouraud (GL_SMOOTH)" : "Flat (GL_FLAT)") << std::endl;
            break;
            
        case 'm':
        case 'M':
            useCube = !useCube;
            loadMesh();
            break;
            
        case 27: // ESC
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mousePressed = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (mousePressed) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;
        
        rotationY += deltaX * 0.5f;
        rotationX += deltaY * 0.5f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

void printInstructions() {
    std::cout << "\n=== Implementação de Gouraud Shading ===" << std::endl;
    std::cout << "Este programa demonstra a diferença entre:" << std::endl;
    std::cout << "- Gouraud Shading (GL_SMOOTH): Normais calculadas por vértice" << std::endl;
    std::cout << "- Flat Shading (GL_FLAT): Normais calculadas por face" << std::endl;
    std::cout << "\nControles:" << std::endl;
    std::cout << "S = Alternar entre Gouraud e Flat Shading" << std::endl;
    std::cout << "M = Alternar entre Cubo e Octaedro" << std::endl;
    std::cout << "Mouse = Arrastar para rotacionar o objeto" << std::endl;
    std::cout << "ESC = Sair" << std::endl;
    std::cout << "\nObservação: A suavização deve ser mais aparente no Gouraud Shading!" << std::endl;
}

int main(int argc, char** argv) {
    printInstructions();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Gouraud Shading - Normais por Vértice");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    
    glutMainLoop();
    
    return 0;
}
