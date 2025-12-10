/*
 * Exercício: Braço Robótico Articulado com Câmera Orbital
 * 
 * Conceitos aplicados:
 * - Modelagem Hierárquica (Base → Braço → Antebraço)
 * - Posicionamento do Observador (gluLookAt com câmera orbital)
 * 
 * NOTA: Este código usa OpenGL "Fixed Function Pipeline" (depreciado)
 * que é adequado para fins educacionais. O OpenGL moderno usa shaders,
 * mas para aprender os conceitos básicos de transformações e hierarquia,
 * o pipeline fixo é mais simples e direto.
 * 
 * Controles:
 * - Q/A: Rotaciona a base (eixo Y)
 * - W/S: Rotaciona o braço (eixo Z)
 * - E/D: Rotaciona o antebraço (eixo Z)
 * - Setas Esquerda/Direita: Controla rotação da câmera
 * - R: Reset das articulações
 * - ESC: Sair
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <cstdio>

// ============================================================
// VARIÁVEIS GLOBAIS
// ============================================================

// Ângulos das articulações do braço robótico
float anguloBase = 0.0f;      // Rotação da base (eixo Y)
float anguloBraco = 45.0f;    // Rotação do braço (eixo Z)
float anguloAntebraco = -30.0f; // Rotação do antebraço (eixo Z)

// Parâmetros da câmera orbital
float cameraAngulo = 45.0f;   // Ângulo de rotação ao redor do robô
float cameraAltura = 8.0f;    // Altura da câmera (observando de cima)
float cameraRaio = 12.0f;     // Distância da câmera ao centro
bool cameraAutoRotate = true; // Rotação automática da câmera

// Dimensões do braço robótico
const float BASE_ALTURA = 1.0f;
const float BASE_RAIO = 1.5f;
const float BRACO_COMPRIMENTO = 4.0f;
const float BRACO_LARGURA = 0.8f;
const float ANTEBRACO_COMPRIMENTO = 3.0f;
const float ANTEBRACO_LARGURA = 0.6f;

// ============================================================
// FUNÇÕES DE DESENHO
// ============================================================

/*
 * Desenha um cubo unitário (1x1x1) centrado na origem
 */
void desenhaCubo() {
    glutSolidCube(1.0);
}

/*
 * Desenha a base do braço robótico
 * - Cilindro fixo no chão
 * - Rotaciona em torno do eixo Y
 */
void desenhaBase() {
    glColor3f(0.3f, 0.3f, 0.8f);  // Azul
    
    // Desenha um cilindro achatado como base
    glPushMatrix();
        glScalef(BASE_RAIO * 2, BASE_ALTURA, BASE_RAIO * 2);
        glTranslatef(0.0f, 0.5f, 0.0f);  // Eleva para ficar sobre o chão
        desenhaCubo();
    glPopMatrix();
}

/*
 * Desenha o braço (segmento que sai da base)
 * - Articulado na junção com a base
 * 
 * IMPORTANTE: Ordem das transformações no código (leitura inversa na matriz)
 * A ordem no código é R → T, mas o efeito é T → R
 * Isso faz o braço rotacionar em torno de sua própria junta (base do braço)
 */
void desenhaBraco() {
    glColor3f(0.8f, 0.3f, 0.3f);  // Vermelho
    
    glPushMatrix();
        // O braço é desenhado como um paralelepípedo alongado
        // Transladamos metade do comprimento para que a rotação seja na extremidade
        glTranslatef(0.0f, BRACO_COMPRIMENTO / 2.0f, 0.0f);
        glScalef(BRACO_LARGURA, BRACO_COMPRIMENTO, BRACO_LARGURA);
        desenhaCubo();
    glPopMatrix();
    
    // Desenha a articulação (esfera na junta)
    glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidSphere(BRACO_LARGURA * 0.6, 16, 16);
}

/*
 * Desenha o antebraço (segmento final)
 * - Articulado na junção com o braço
 */
void desenhaAntebraco() {
    glColor3f(0.3f, 0.8f, 0.3f);  // Verde
    
    glPushMatrix();
        // Similar ao braço, centralizamos para rotação na junta
        glTranslatef(0.0f, ANTEBRACO_COMPRIMENTO / 2.0f, 0.0f);
        glScalef(ANTEBRACO_LARGURA, ANTEBRACO_COMPRIMENTO, ANTEBRACO_LARGURA);
        desenhaCubo();
    glPopMatrix();
    
    // Desenha a articulação
    glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidSphere(ANTEBRACO_LARGURA * 0.6, 16, 16);
    
    // Desenha a "garra" no final (esfera)
    glPushMatrix();
        glTranslatef(0.0f, ANTEBRACO_COMPRIMENTO, 0.0f);
        glColor3f(0.8f, 0.8f, 0.3f);  // Amarelo
        glutSolidSphere(ANTEBRACO_LARGURA * 0.8, 16, 16);
    glPopMatrix();
}

/*
 * Desenha o chão (grid para referência visual)
 */
void desenhaChao() {
    glColor3f(0.4f, 0.4f, 0.4f);
    
    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++) {
        // Linhas paralelas ao eixo X
        glVertex3f(-10.0f, 0.0f, (float)i);
        glVertex3f(10.0f, 0.0f, (float)i);
        // Linhas paralelas ao eixo Z
        glVertex3f((float)i, 0.0f, -10.0f);
        glVertex3f((float)i, 0.0f, 10.0f);
    }
    glEnd();
}

/*
 * Desenha os eixos coordenados para referência
 */
void desenhaEixos() {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        // Eixo X - Vermelho
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(3.0f, 0.0f, 0.0f);
        
        // Eixo Y - Verde
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 3.0f, 0.0f);
        
        // Eixo Z - Azul
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 3.0f);
    glEnd();
    glLineWidth(1.0f);
}

// ============================================================
// MODELAGEM HIERÁRQUICA DO BRAÇO ROBÓTICO
// ============================================================

/*
 * Desenha o braço robótico completo usando modelagem hierárquica
 * 
 * Hierarquia:
 *   Base (Pai)
 *     └── Braço (Filho)
 *           └── Antebraço (Neto)
 * 
 * Conceito chave: glPushMatrix/glPopMatrix preservam o estado da matriz
 * para que transformações de filhos não afetem os pais ou irmãos.
 * 
 * RESPOSTA À QUESTÃO 3:
 * A ordem no código é R → T (primeiro rotação, depois translação),
 * mas como OpenGL usa pré-multiplicação de matrizes, o efeito
 * é T aplicado primeiro, depois R. Isso permite que o objeto
 * rotacione em torno de sua junta local, não da origem global.
 */
void desenhaBracoRobotico() {
    // ============================================================
    // NÍVEL 0: BASE (Pai de todos)
    // ============================================================
    // RESPOSTA À QUESTÃO 2:
    // Para desenhar a base, usamos glPushMatrix para salvar o estado,
    // aplicamos a rotação da base (eixo Y), desenhamos, e mantemos
    // a matriz para os filhos herdarem a transformação.
    
    glPushMatrix();  // Salva estado inicial (matriz de visualização)
        
        // Rotação da base em torno do eixo Y
        glRotatef(anguloBase, 0.0f, 1.0f, 0.0f);
        
        // Desenha a base
        desenhaBase();
        
        // ============================================================
        // NÍVEL 1: BRAÇO (Filho da Base)
        // ============================================================
        glPushMatrix();  // Salva estado após rotação da base
            
            // Translada para o topo da base (ponto de articulação)
            glTranslatef(0.0f, BASE_ALTURA, 0.0f);
            
            // Rotação do braço em torno do eixo Z (articulação)
            // ORDEM: R → T no código = T → R na aplicação
            glRotatef(anguloBraco, 0.0f, 0.0f, 1.0f);
            
            // Desenha o braço
            desenhaBraco();
            
            // ============================================================
            // NÍVEL 2: ANTEBRAÇO (Filho do Braço)
            // ============================================================
            glPushMatrix();  // Salva estado após transformações do braço
                
                // Translada para a ponta do braço (próxima articulação)
                glTranslatef(0.0f, BRACO_COMPRIMENTO, 0.0f);
                
                // Rotação do antebraço em torno do eixo Z
                glRotatef(anguloAntebraco, 0.0f, 0.0f, 1.0f);
                
                // Desenha o antebraço
                desenhaAntebraco();
                
            glPopMatrix();  // Restaura estado do braço
            
        glPopMatrix();  // Restaura estado da base
        
    glPopMatrix();  // Restaura estado inicial
}

// ============================================================
// CONFIGURAÇÃO DA CÂMERA
// ============================================================

/*
 * Configura a câmera orbital usando gluLookAt
 * 
 * RESPOSTA À QUESTÃO 1:
 * Para observar o braço robótico de cima, girando ao redor dele:
 * 
 * Eye (posição da câmera):
 *   - X = raio * cos(ângulo)  → posição circular no plano XZ
 *   - Y = altura              → elevada para ver de cima
 *   - Z = raio * sin(ângulo)  → posição circular no plano XZ
 * 
 * Center (ponto de interesse):
 *   - (0, 2, 0) → centro aproximado do braço robótico
 *   - Y = 2 para olhar para o meio do braço, não para o chão
 * 
 * Up (vetor "para cima"):
 *   - (0, 1, 0) → Y positivo é sempre "para cima"
 */
void configuraCameraOrbital() {
    // Converte ângulo para radianos
    float angRad = cameraAngulo * M_PI / 180.0f;
    
    // Calcula posição da câmera em coordenadas esféricas
    // Eye: posição da câmera (girando ao redor do eixo Y)
    float eyeX = cameraRaio * cos(angRad);
    float eyeY = cameraAltura;
    float eyeZ = cameraRaio * sin(angRad);
    
    // Center: ponto para onde a câmera olha (centro do braço)
    float centerX = 0.0f;
    float centerY = 2.0f;  // Altura média do braço
    float centerZ = 0.0f;
    
    // Up: vetor que define "para cima" da câmera
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;
    
    gluLookAt(eyeX, eyeY, eyeZ,      // Eye (posição da câmera)
              centerX, centerY, centerZ,  // Center (ponto de interesse)
              upX, upY, upZ);             // Up (vetor "para cima")
}

// ============================================================
// CALLBACKS DO GLUT
// ============================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Configura a câmera orbital
    configuraCameraOrbital();
    
    // Desenha elementos da cena
    desenhaChao();
    desenhaEixos();
    desenhaBracoRobotico();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Timer para rotação automática da câmera
 */
void timer(int) {
    if (cameraAutoRotate) {
        cameraAngulo += 0.5f;  // Rotação lenta
        if (cameraAngulo >= 360.0f) cameraAngulo -= 360.0f;
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // ~60 FPS
}

/*
 * Teclado - controles das articulações e câmera
 */
void keyboard(unsigned char key, int, int) {
    switch (key) {
        // Controle da Base (eixo Y)
        case 'q': case 'Q':
            anguloBase += 5.0f;
            break;
        case 'a': case 'A':
            anguloBase -= 5.0f;
            break;
            
        // Controle do Braço (eixo Z)
        case 'w': case 'W':
            anguloBraco += 5.0f;
            if (anguloBraco > 90.0f) anguloBraco = 90.0f;
            break;
        case 's': case 'S':
            anguloBraco -= 5.0f;
            if (anguloBraco < -90.0f) anguloBraco = -90.0f;
            break;
            
        // Controle do Antebraço (eixo Z)
        case 'e': case 'E':
            anguloAntebraco += 5.0f;
            if (anguloAntebraco > 0.0f) anguloAntebraco = 0.0f;
            break;
        case 'd': case 'D':
            anguloAntebraco -= 5.0f;
            if (anguloAntebraco < -150.0f) anguloAntebraco = -150.0f;
            break;
            
        // Toggle rotação automática da câmera
        case ' ':
            cameraAutoRotate = !cameraAutoRotate;
            break;
            
        // Reset
        case 'r': case 'R':
            anguloBase = 0.0f;
            anguloBraco = 45.0f;
            anguloAntebraco = -30.0f;
            cameraAngulo = 45.0f;
            break;
            
        // Sair
        case 27:  // ESC
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

/*
 * Teclas especiais - controle manual da câmera
 */
void specialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraAngulo -= 5.0f;
            cameraAutoRotate = false;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngulo += 5.0f;
            cameraAutoRotate = false;
            break;
        case GLUT_KEY_UP:
            cameraAltura += 0.5f;
            if (cameraAltura > 15.0f) cameraAltura = 15.0f;
            break;
        case GLUT_KEY_DOWN:
            cameraAltura -= 0.5f;
            if (cameraAltura < 2.0f) cameraAltura = 2.0f;
            break;
    }
    
    glutPostRedisplay();
}

/*
 * Inicialização do OpenGL
 */
void init() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);  // Fundo escuro
    
    glEnable(GL_DEPTH_TEST);
    
    // Iluminação básica para melhor visualização 3D
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_position[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
    printf("\n");
    printf("===========================================\n");
    printf("   BRAÇO ROBÓTICO - Modelagem Hierárquica\n");
    printf("===========================================\n");
    printf("\n");
    printf("CONTROLES:\n");
    printf("  Q/A     - Rotaciona a BASE (eixo Y)\n");
    printf("  W/S     - Rotaciona o BRAÇO (eixo Z)\n");
    printf("  E/D     - Rotaciona o ANTEBRAÇO (eixo Z)\n");
    printf("  Setas   - Controle manual da câmera\n");
    printf("  ESPAÇO  - Liga/desliga rotação automática\n");
    printf("  R       - Reset\n");
    printf("  ESC     - Sair\n");
    printf("\n");
    printf("===========================================\n");
    printf("\n");
}

// ============================================================
// MAIN
// ============================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Braço Robótico - Modelagem Hierárquica");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);
    
    glutMainLoop();
    
    return 0;
}
