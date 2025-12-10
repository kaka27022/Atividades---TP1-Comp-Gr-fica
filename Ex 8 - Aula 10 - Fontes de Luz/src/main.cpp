#include <GL/glut.h>
#include <cmath>
#include <cstdio>

// -------------------------
// Controles do observador
// -------------------------
float camX = 0, camY = 0, camZ = 5;   // posição da câmera
float yaw = 0;      // giro esquerda/direita
float pitch = 0;    // giro cima/baixo
float moveSpeed = 0.2f;

// ----------------------------------------------------
// Função utilitária para normalizar vetor (p/ spotlight)
// ----------------------------------------------------
void normalize(float v[3]) {
    float len = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if(len != 0){
        v[0] /= len; v[1] /= len; v[2] /= len;
    }
}

// ----------------------------------------------------
// Desenha cubo branco com normais corretas
// ----------------------------------------------------
void drawCube()
{
    glBegin(GL_QUADS);

    glColor3f(1,1,1);  // CUBO BRANCO

    // Frente
    glNormal3f(0,0,1);
    glVertex3f(-1,-1, 1);
    glVertex3f( 1,-1, 1);
    glVertex3f( 1, 1, 1);
    glVertex3f(-1, 1, 1);

    // Trás
    glNormal3f(0,0,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1, 1,-1);
    glVertex3f( 1, 1,-1);
    glVertex3f( 1,-1,-1);

    // Esquerda
    glNormal3f(-1,0,0);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, 1,-1);

    // Direita
    glNormal3f(1,0,0);
    glVertex3f( 1,-1,-1);
    glVertex3f( 1, 1,-1);
    glVertex3f( 1, 1, 1);
    glVertex3f( 1,-1, 1);

    // Topo
    glNormal3f(0,1,0);
    glVertex3f(-1, 1,-1);
    glVertex3f(-1, 1, 1);
    glVertex3f( 1, 1, 1);
    glVertex3f( 1, 1,-1);

    // Base
    glNormal3f(0,-1,0);
    glVertex3f(-1,-1,-1);
    glVertex3f( 1,-1,-1);
    glVertex3f( 1,-1, 1);
    glVertex3f(-1,-1, 1);

    glEnd();
}

// ----------------------------------------------------
// Configura spotlight AMARELO seguindo o observador
// ----------------------------------------------------
void setupLight()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Cor AMARELA
    GLfloat lightDiffuse[]  = {1.0f, 1.0f, 0.4f, 1};
    GLfloat lightAmbient[]  = {0.25f, 0.25f, 0.15f, 1}; // ambiente leve
    GLfloat lightSpecular[] = {1,1,0.5f,1};

    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Posição no espaço do OBSERVADOR
    GLfloat pos[] = {0,0,0,1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    // -------------------------------
    // Spotlight: direção do observador
    // -------------------------------
    float yawR   = yaw   * M_PI/180.0f;
    float pitchR = pitch * M_PI/180.0f;

    float dir[3] = {
        cosf(pitchR) * sinf(yawR),
        sinf(pitchR),
        -cosf(pitchR) * cosf(yawR)
    };
    normalize(dir);

    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

    // CUTOFF + EXPONENT pedidos na atividade
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF,   30.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 15.0f);

    // Atenuação quadrática
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);
}

// ----------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matriz identidade
    glLoadIdentity();

    // --- Luz no espaço do observador (antes da câmera!) ---
    setupLight();   // a luz agora está presa à câmera

    // Agora aplica a câmera
    gluLookAt(camX, camY, camZ,
            camX + cos(pitch*M_PI/180.0)*sin(yaw*M_PI/180.0),
            camY + sin(pitch*M_PI/180.0),
            camZ - cos(pitch*M_PI/180.0)*cos(yaw*M_PI/180.0),
            0,1,0);

    // Desenha o cubo
    drawCube();

    glutSwapBuffers();
}

void reshape(int w,int h){
    if(h==0) h=1;
    float aspect = (float)w/h;

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, aspect, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key){
        case 'w': camZ -= moveSpeed * cos(yaw*M_PI/180); camX += moveSpeed * sin(yaw*M_PI/180); break;
        case 's': camZ += moveSpeed * cos(yaw*M_PI/180); camX -= moveSpeed * sin(yaw*M_PI/180); break;
        case 'a': camX -= moveSpeed * cos(yaw*M_PI/180); camZ -= moveSpeed * sin(yaw*M_PI/180); break;
        case 'd': camX += moveSpeed * cos(yaw*M_PI/180); camZ += moveSpeed * sin(yaw*M_PI/180); break;
        case 'q': camY += moveSpeed; break;
        case 'e': camY -= moveSpeed; break;
        case 27: exit(0);
    }
}
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT)  yaw -= 2;
    if(key == GLUT_KEY_RIGHT) yaw += 2;
    if(key == GLUT_KEY_UP)    pitch += 2;
    if(key == GLUT_KEY_DOWN)  pitch -= 2;
}

// ----------------------------------------------------
void init()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // permite que glColor funcione com iluminação
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // material básico (para specular)
    GLfloat matSpecular[] = {1,1,1,1};
    GLfloat shininess[]   = {50};

    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glClearColor(0.05,0.05,0.07,1);
}

// ----------------------------------------------------
int main(int argc,char** argv)
{
    printf("Controles:\nW/S = mover frente/tras\nA/D = mover lateral\nQ/E = subir/descer\nSetas = olhar\nESC = sair\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900,700);
    glutCreateWindow("Ex 10 - Fontes de Luz (Lanterna do Observador)");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}
