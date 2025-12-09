#include <GL/glut.h>
#include "cubo.h"

Cubo::Cubo(float l) {
    lado = l;
}

void Cubo::desenhar() {
    glutWireCube(lado);
}
