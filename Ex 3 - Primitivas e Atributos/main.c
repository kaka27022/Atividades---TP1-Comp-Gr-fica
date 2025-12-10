/*
Aula 5

*Exercício para Casa: Otimizando o Quadrado*

*Objetivo: Desenho Eficiente e Colorido*
Implemente uma função em C/OpenGL (Pipeline Fixo) que desenhe um quadrado (na verdade, dois triângulos) no plano XY, utilizando a abordagem mais eficiente em termos de número de vértices.

*Requisitos de Implementação:*
** Utilize o modo de desenho **GL_TRIANGLE_STRIP**.
** Defina **apenas 4 vértices** para desenhar a figura.
** Atribua uma **cor única** a cada um dos 4 vértices (e.g., Vermelho, Verde, Azul, Amarelo).

**Desafio Conceitual (Bônus)**
Responda: Por que a figura final exibida na tela apresentará um gradiente de cores suave (e não cores sólidas) dentro de sua área? *(Conecte com o conceito de interpolação e o Sombreamento Gouraud).*
*/

#include <GL/freeglut.h>

// Função de display que desenha a cena
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 3. Desenha o quadrado usando GL_TRIANGLE_STRIP
    glBegin(GL_TRIANGLE_STRIP);
        
        //vertice 1 vermelho
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, 0.5f); 

        //vertice 2 verde
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f); 

        //vertice 3 azul
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.5f, 0.5f);

        //vertice 4 amarelo
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.5f, -0.5f);
        
    glEnd();

    glutSwapBuffers();
}

//iniciação do opengl
void initGL()
{
    //cor cinza no fundo
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

int main(int argc, char** argv)
{
    //inicialização do glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Quadrado Otimizado (GL_TRIANGLE_STRIP)");

    //funções de callback
    glutDisplayFunc(display);
    
    //configura o estado inicial do opengl
    initGL();

    //entra no loop principal de eventos
    glutMainLoop();
    
    return 0;
}

/*
Resposta da pergunta bônus:
A figura exibida na tela apresentará um gradiente de cores suave (e não quatro cores sólidas separadas) devido ao conceito de interpolação de atributos na rasterização, especificamente o Sombreamento Gouraud (Gouraud Shading).
*/