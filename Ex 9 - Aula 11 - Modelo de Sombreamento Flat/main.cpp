#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <iostream>
#include <cmath>

// Variáveis globais
bool isFlat = true;  // true = GL_FLAT, false = GL_SMOOTH
bool drawCube = false; // true = cubo, false = esfera (começar com esfera para ver diferenças)
bool autoRotate = true; // rotação automática para melhor visualização
float rotationX = 30.0f;
float rotationY = 0.0f;
float autoRotationY = 0.0f;

// Função para configurar a iluminação
void setupLighting() {
    // Habilita iluminação
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Configura propriedades da luz
    GLfloat light_position[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Habilita uso de glColor como material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Configura material especular
    GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

// Função para desenhar o cubo colorido
void drawColoredCube() {
    glBegin(GL_QUADS);
    
    // Face frontal - Vermelha
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    
    // Face traseira - Verde
    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Face superior - Azul
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Face inferior - Amarela
    glColor3f(1.0f, 1.0f, 0.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Face direita - Magenta
    glColor3f(1.0f, 0.0f, 1.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    
    // Face esquerda - Ciano
    glColor3f(0.0f, 1.0f, 1.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    
    glEnd();
}

// Função para desenhar objeto que mostra diferenças entre GL_FLAT e GL_SMOOTH
void drawColoredSphere() {
    // Vamos desenhar múltiplas esferas com cores diferentes para mostrar as diferenças
    
    // Esfera central grande
    glColor3f(1.0f, 0.5f, 0.0f);  // Laranja
    glutSolidSphere(1.2, 15, 10);  // Poucos segmentos para ver diferenças
    
    // Esferas menores ao redor
    glPushMatrix();
    glTranslatef(2.5f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.5f);  // Verde claro
    glutSolidSphere(0.6, 10, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-2.5f, 0.0f, 0.0f);
    glColor3f(0.5f, 0.0f, 1.0f);  // Roxo
    glutSolidSphere(0.6, 10, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 2.5f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.5f);  // Rosa
    glutSolidSphere(0.6, 10, 8);
    glPopMatrix();
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Define o modelo de sombreamento
    if (isFlat) {
        glShadeModel(GL_FLAT);
    } else {
        glShadeModel(GL_SMOOTH);
    }
    
    glLoadIdentity();
    
    // Posiciona a câmera
    gluLookAt(5.0, 5.0, 5.0,   // posição da câmera
              0.0, 0.0, 0.0,   // ponto para onde olha
              0.0, 1.0, 0.0);  // vetor up
    
    // Aplica rotações para melhor visualização
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    
    // Rotação automática para visualizar diferenças
    if (autoRotate) {
        autoRotationY += 1.0f;
        if (autoRotationY >= 360.0f) autoRotationY = 0.0f;
        glRotatef(autoRotationY, 0.0f, 1.0f, 0.0f);
    }
    
    // Desenha o objeto selecionado
    if (drawCube) {
        drawColoredCube();
    } else {
        drawColoredSphere();
    }
    
    glutPostRedisplay();
    glutSwapBuffers();
}

// Função de redimensionamento da janela
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Callback de teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'f':
        case 'F':
            isFlat = true;
            std::cout << "\n=== MUDANÇA DE MODO ===" << std::endl;
            std::cout << ">>> GL_FLAT ATIVADO <<<" << std::endl;
            std::cout << "- Sombreamento uniforme por face" << std::endl;
            std::cout << "- Bordas definidas entre faces" << std::endl;
            std::cout << "======================" << std::endl;
            break;
        case 'g':
        case 'G':
            isFlat = false;
            std::cout << "\n=== MUDANÇA DE MODO ===" << std::endl;
            std::cout << ">>> GL_SMOOTH ATIVADO <<<" << std::endl;
            std::cout << "- Sombreamento suave com interpolação" << std::endl;
            std::cout << "- Transições graduais entre faces" << std::endl;
            std::cout << "========================" << std::endl;
            break;
        case 'c':
        case 'C':
            drawCube = !drawCube;
            std::cout << "Objeto alterado para: " << (drawCube ? "CUBO" : "ESFERA") << std::endl;
            break;
        case 'r':
        case 'R':
            autoRotate = !autoRotate;
            std::cout << "Rotação automática: " << (autoRotate ? "LIGADA" : "DESLIGADA") << std::endl;
            break;
        case 27: // ESC
            std::cout << "Saindo..." << std::endl;
            exit(0);
            break;
        default:
            std::cout << "Teclas disponíveis: F(flat), G(smooth), C(cubo/esfera), R(rotação), ESC(sair)" << std::endl;
            break;
    }
}

// Callback para teclas especiais (setas para rotação)
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            rotationX -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            rotationX += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            rotationY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            rotationY += 5.0f;
            break;
    }
}

// Função de inicialização
void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    setupLighting();
    
    std::cout << "=== Comparação GL_FLAT vs GL_SMOOTH ===" << std::endl;
    std::cout << "IMPORTANTE: As diferenças são mais visíveis nas ESFERAS!" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "F - Modo GL_FLAT (facetado, bordas definidas)" << std::endl;
    std::cout << "G - Modo GL_SMOOTH (suave, sem bordas)" << std::endl;
    std::cout << "C - Alternar entre esferas e cubo colorido" << std::endl;
    std::cout << "R - Liga/desliga rotação automática" << std::endl;
    std::cout << "Setas - Rotação manual" << std::endl;
    std::cout << "ESC - Sair" << std::endl;
    std::cout << "" << std::endl;
    std::cout << ">>> TESTE F e G nas ESFERAS para ver as diferenças! <<<" << std::endl;
    std::cout << ">>> GL_FLAT = Aparência facetada (como diamante) <<<" << std::endl;
    std::cout << ">>> GL_SMOOTH = Aparência lisa e suave <<<" << std::endl;
    std::cout << "=================================================" << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Comparação GL_FLAT vs GL_SMOOTH - Computação Gráfica");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutMainLoop();
    return 0;
}
