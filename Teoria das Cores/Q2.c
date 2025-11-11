#include <GL/gl.h>
#include <GL/glut.h>

void display(void) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // fundo cinza
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.4f, 0.8f, 0.8f); // cor convertida de HSV

    glBegin(GL_TRIANGLES);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.5f, -0.5f);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Triangulo com cor HSV convertida");

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

// --------------------- Compilar ----------------------
// gcc main.c -o triangulo -lGL -lGLU -lglut
// ./triangulo