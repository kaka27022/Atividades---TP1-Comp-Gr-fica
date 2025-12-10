// Cubo com 6 cores (1 por face), triangulado, com rotação completa.
// Pipeline Fixo (OpenGL antigo), GLUT.

// ----------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------
#include <GL/glut.h>

// ----------------------------------------------------------
// Ângulos de rotação em dois eixos (para mostrar TODAS as faces)
// ----------------------------------------------------------
float angX = 0.0f;
float angY = 0.0f;

// ----------------------------------------------------------
// Vértices do cubo – total 8
// ----------------------------------------------------------
GLfloat V[8][3] = {
    {-0.5f, -0.5f, -0.5f}, // 0
    { 0.5f, -0.5f, -0.5f}, // 1
    { 0.5f,  0.5f, -0.5f}, // 2
    {-0.5f,  0.5f, -0.5f}, // 3
    {-0.5f, -0.5f,  0.5f}, // 4
    { 0.5f, -0.5f,  0.5f}, // 5
    { 0.5f,  0.5f,  0.5f}, // 6
    {-0.5f,  0.5f,  0.5f}  // 7
};

// ----------------------------------------------------------
// 12 triângulos (2 por face) → índice dos vértices
// ----------------------------------------------------------
GLubyte faces[6][6] = {
    {4,5,6, 4,6,7}, // frente
    {0,3,2, 0,2,1}, // trás
    {1,2,6, 1,6,5}, // direita
    {0,4,7, 0,7,3}, // esquerda
    {3,7,6, 3,6,2}, // topo
    {0,1,5, 0,5,4}  // base
};

// ----------------------------------------------------------
// Cores por face (uma cor sólida por face)
// ----------------------------------------------------------
GLfloat cores[6][3] = {
    {1,0,0}, // frente vermelha
    {0,1,0}, // trás verde
    {0,0,1}, // direita azul
    {1,1,0}, // esquerda amarela
    {1,0,1}, // topo magenta
    {0,1,1}  // base ciano
};

// ----------------------------------------------------------
// Normais por face (para iluminação básica)
// ----------------------------------------------------------
GLfloat normals[6][3] = {
    { 0, 0, 1},   // frente
    { 0, 0,-1},   // trás
    { 1, 0, 0},   // direita
    {-1, 0, 0},   // esquerda
    { 0, 1, 0},   // topo
    { 0,-1, 0}    // base
};

// ----------------------------------------------------------
// Desenha o cubo triangulado
// ----------------------------------------------------------
void desenhaCubo() {
    glBegin(GL_TRIANGLES);

    for(int f = 0; f < 6; f++) {

        glColor3fv(cores[f]);     // muda cor por face
        glNormal3fv(normals[f]);  // normal da face

        for(int v = 0; v < 6; v++) {
            glVertex3fv(V[ faces[f][v] ]); // 6 índices → 2 triângulos
        }
    }

    glEnd();
}

// ----------------------------------------------------------
// Função de desenho principal
// ----------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Define câmera
    gluLookAt(2, 2, 4,  0,0,0,  0,1,0);

    // Reposiciona a luz (precisa ser feito aqui!)
    GLfloat pos[4] = {2, 2, 2, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    // Rotação em dois eixos para mostrar TODAS as faces
    glRotatef(angX, 1, 0, 0);
    glRotatef(angY, 0, 1, 0);

    // Desenha o cubo
    desenhaCubo();

    glutSwapBuffers();
}

// ----------------------------------------------------------
// Timer: aumenta os ângulos gradualmente
// ----------------------------------------------------------
void timer(int) {
    angX += 1.0f;     // gira no eixo X
    angY += 0.7f;     // gira no eixo Y

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// ----------------------------------------------------------
// Configurações iniciais
// ----------------------------------------------------------
void init() {
    glEnable(GL_DEPTH_TEST);  // habilita profundidade

    // ---------- Iluminação ----------
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // permite que glColor defina a cor do material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // luz ambiente global (para não deixar faces escuras)
    GLfloat luzAmb[4] = {0.4, 0.4, 0.4, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmb);

    glShadeModel(GL_FLAT); // cor uniforme por face
}

// ----------------------------------------------------------
// Ajuste da janela
// ----------------------------------------------------------
void reshape(int w, int h) {
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, (float)w/h, 0.1, 50);

    glMatrixMode(GL_MODELVIEW);
}

// ----------------------------------------------------------
// Main
// ----------------------------------------------------------
int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("Cubo Colorido - Rotação Completa");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
