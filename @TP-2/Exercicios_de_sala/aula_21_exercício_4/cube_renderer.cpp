#include "cube_renderer.h"

void desenhaCubo() {
    glBegin(GL_QUADS);
    
    // Face Frontal (Z+) - Vermelha
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); 
    glVertex3f( 1.0f, -1.0f,  1.0f); 
    glVertex3f( 1.0f,  1.0f,  1.0f); 
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Face Traseira (Z-) - Verde
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); 
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); 

    // Face Superior (Y+) - Azul
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f); 
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); 
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // Face Inferior (Y-) - Amarela
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); 
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); 
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Face Lateral Direita (X+) - Magenta
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f); 
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); 
    glVertex3f( 1.0f,  1.0f,  1.0f);

    // Face Lateral Esquerda (X-) - Ciano
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); 
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); 
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glEnd();
}