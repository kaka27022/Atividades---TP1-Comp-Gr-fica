/*
Aula 13

**Exercício Proposto**

**Tarefa:** Altere um código associado à *Gouraud Shading* para demonstrar a diferença nos destaques especulares.

**Passos Sugeridos**

**Setup:** Utilize um objeto de baixa poligonagem (cubo ou esfera simples) com normais de vértice pré-calculadas.

**Material Especular Alto:** Defina **GL_SPECULAR** como branco e **GL_SHININESS** com um valor alto (≥ 50.0) para forçar um destaque pequeno.

**Demonstração:** Renderize a cena e mova a posição da luz (**GL_POSITION**) ou a posição da câmera.

**Observação:** Discuta por que o destaque especular (o brilho) no Gouraud Shading (**GL_SMOOTH**) parece “correr” ou desaparecer de forma não natural, ilustrando a necessidade do Shading de Phong.
*/

#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

float light_pos[] = {5.0f, 5.0f, 5.0f, 1.0f};

GLfloat light_ambient[]  = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

GLfloat mat_diffuse[]  = {0.7f, 0.0f, 0.0f, 1.0f};
GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat mat_shininess  = 100.0f;


void setup_lighting() {

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,1);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); 
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf (GL_FRONT, GL_SHININESS, mat_shininess); 
    
    glEnable(GL_DEPTH_TEST); 
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos); 

    glutSolidSphere(1.0, 15, 15); 

    glutSwapBuffers();
}

void timer(int value) {
    float t = glutGet(GLUT_ELAPSED_TIME) / 2000.0f;

    light_pos[0] = 5.0f * sin(t);
    light_pos[2] = 5.0f * cos(t);

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 1.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Gouraud Shading: Destaques Especulares");

    setup_lighting();

    glutDisplayFunc(display);
    
    glutReshapeFunc(reshape); 

    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}

/*
Discussão

O problema do Gouraud Shading ocorre porque ele calcula a iluminação, incluindo o brilho especular,apenas nos vértices dos polígonos, e depois interpola linearmente essas cores para o interior das faces. Quando o destaque especular é muito pequeno e concentrado (por exemplo, com GL_SHININESS alto), ele normalmente cai entre os vértices, e portanto não é calculado em nenhum deles. Assim, o brilho pode simplesmente não aparecer ou surgir de forma abrupta quando, por acaso, a luz ou a câmera alinham o destaque exatamente com um vértice. Isso faz o brilho “pular” ou “correr” entre as faces da malha à medida que a luz se move.

O Phong Shading resolve esse problema ao inverter a lógica: em vez de interpolar cores prontas, ele interpola as normais e calcula toda a iluminação por pixel. Isso permite determinar corretamente a posição e intensidade do brilho especular mesmo quando ele cai entre vértices, produzindo destaques muito mais naturais, suaves e estáveis. Por isso, o Phong Shading é usado quando se deseja realismo em superfícies brilhantes.
*/