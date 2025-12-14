#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

// Estrutura para representar um ponto 3D
struct Ponto3D {
    double x, y, z;
    
    Ponto3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    void print(const std::string& nome) const {
        std::cout << nome << "(" << std::fixed << std::setprecision(1) 
                  << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

// Classe para matriz 4x4 (transformações homogêneas)
class Matriz4x4 {
private:
    double m[4][4];
    
public:
    Matriz4x4() {
        // Inicializa como matriz identidade
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                m[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }
    }
    
    // Multiplicação de matrizes
    Matriz4x4 operator*(const Matriz4x4& other) const {
        Matriz4x4 result;
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                result.m[i][j] = 0;
                for(int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
    
    // Aplicar transformação a um ponto
    Ponto3D transformar(const Ponto3D& p) const {
        double x = m[0][0]*p.x + m[0][1]*p.y + m[0][2]*p.z + m[0][3];
        double y = m[1][0]*p.x + m[1][1]*p.y + m[1][2]*p.z + m[1][3];
        double z = m[2][0]*p.x + m[2][1]*p.y + m[2][2]*p.z + m[2][3];
        return Ponto3D(x, y, z);
    }
    
    // Definir matriz de rotação em X
    void rotacaoX(double angulo) {
        double rad = angulo * M_PI / 180.0;
        double cos_a = cos(rad);
        double sin_a = sin(rad);
        
        m[0][0] = 1; m[0][1] = 0;     m[0][2] = 0;      m[0][3] = 0;
        m[1][0] = 0; m[1][1] = cos_a; m[1][2] = -sin_a; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = sin_a; m[2][2] = cos_a;  m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0;     m[3][2] = 0;      m[3][3] = 1;
    }
    
    // Definir matriz de rotação em Y
    void rotacaoY(double angulo) {
        double rad = angulo * M_PI / 180.0;
        double cos_a = cos(rad);
        double sin_a = sin(rad);
        
        m[0][0] = cos_a;  m[0][1] = 0; m[0][2] = sin_a; m[0][3] = 0;
        m[1][0] = 0;      m[1][1] = 1; m[1][2] = 0;     m[1][3] = 0;
        m[2][0] = -sin_a; m[2][1] = 0; m[2][2] = cos_a; m[2][3] = 0;
        m[3][0] = 0;      m[3][1] = 0; m[3][2] = 0;     m[3][3] = 1;
    }
    
    // Definir matriz de translação
    void translacao(double tx, double ty, double tz) {
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = tx;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = ty;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = tz;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
    }
    
    // Definir matriz de escala
    void escala(double sx, double sy, double sz) {
        m[0][0] = sx; m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
        m[1][0] = 0;  m[1][1] = sy; m[1][2] = 0;  m[1][3] = 0;
        m[2][0] = 0;  m[2][1] = 0;  m[2][2] = sz; m[2][3] = 0;
        m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;
    }
    
    // Imprimir matriz (para debug)
    void print() const {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                std::cout << std::fixed << std::setprecision(3) << std::setw(8) << m[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Problema 1: Rotação em torno do eixo X por 180°
void problema1() {
    std::cout << "=== PROBLEMA 1 - Rotação em X por 180° ===" << std::endl;
    std::cout << "Revisão Algébrica" << std::endl;
    
    // Pontos originais
    Ponto3D A(1, 0, 0);
    Ponto3D B(0, 1, 0);
    Ponto3D C(0, 0, 1);
    
    std::cout << "\nPontos originais:" << std::endl;
    A.print("A");
    B.print("B");
    C.print("C");
    
    // Matriz de rotação em X por 180°
    Matriz4x4 rotX;
    rotX.rotacaoX(180.0);
    
    std::cout << "\nMatriz de rotação em X por 180°:" << std::endl;
    rotX.print();
    
    // Aplicar transformação
    Ponto3D A_novo = rotX.transformar(A);
    Ponto3D B_novo = rotX.transformar(B);
    Ponto3D C_novo = rotX.transformar(C);
    
    std::cout << "\nPontos após rotação de 180° em X:" << std::endl;
    A_novo.print("A'");
    B_novo.print("B'");
    C_novo.print("C'");
    
    // Verificação da resposta esperada
    std::cout << "\nResposta esperada: A'(1,0,0), B'(0,-1,0), C'(0,0,-1)" << std::endl;
    std::cout << std::endl;
}

// Problema 2: Composição de transformações
void problema2() {
    std::cout << "=== PROBLEMA 2 - Composição de Transformações ===" << std::endl;
    
    std::cout << "Sequência de operações desejadas:" << std::endl;
    std::cout << "1. Rotacionar o objeto 45° em torno do seu eixo local Y" << std::endl;
    std::cout << "2. Mover o objeto 10 unidades para a frente (eixo Z)" << std::endl;
    std::cout << "3. Escalar o objeto por um fator de 2.0" << std::endl;
    
    std::cout << "\nDica: O que deve ser aplicado primeiro ao vértice (na direita)" << std::endl;
    std::cout << "deve ser chamado por último no código (na esquerda)." << std::endl;
    
    std::cout << "\nSequência CORRETA de chamadas OpenGL:" << std::endl;
    std::cout << "glScaled(2.0, 2.0, 2.0);     // 3° operação (aplicada por último)" << std::endl;
    std::cout << "glTranslatef(0.0, 0.0, 10.0); // 2° operação" << std::endl;
    std::cout << "glRotatef(45.0, 0, 1, 0);     // 1° operação (aplicada primeiro)" << std::endl;
    
    std::cout << "\n--- Implementação e Verificação ---" << std::endl;
    
    // Criar as matrizes individuais
    Matriz4x4 rotY, translacao, escala;
    
    rotY.rotacaoY(45.0);
    translacao.translacao(0.0, 0.0, 10.0);
    escala.escala(2.0, 2.0, 2.0);
    
    std::cout << "\nMatriz de Rotação Y 45°:" << std::endl;
    rotY.print();
    
    std::cout << "\nMatriz de Translação (0, 0, 10):" << std::endl;
    translacao.print();
    
    std::cout << "\nMatriz de Escala (2, 2, 2):" << std::endl;
    escala.print();
    
    // Composição: Escala * Translação * Rotação
    // (ordem das multiplicações é inversa à ordem de aplicação)
    Matriz4x4 transformacao = escala * translacao * rotY;
    
    std::cout << "\nMatriz de Transformação Composta (E * T * R):" << std::endl;
    transformacao.print();
    
    // Teste com um ponto exemplo
    Ponto3D pontoTeste(1, 0, 0);
    std::cout << "\nTeste com ponto (1, 0, 0):" << std::endl;
    pontoTeste.print("Ponto original");
    
    // Aplicar transformações passo a passo
    Ponto3D p1 = rotY.transformar(pontoTeste);
    p1.print("Após rotação Y 45°");
    
    Ponto3D p2 = translacao.transformar(p1);
    p2.print("Após translação Z +10");
    
    Ponto3D p3 = escala.transformar(p2);
    p3.print("Após escala 2.0");
    
    // Aplicar transformação composta
    Ponto3D resultado = transformacao.transformar(pontoTeste);
    resultado.print("Resultado da composição");
    
    std::cout << "\nVerificação: Os dois últimos resultados devem ser iguais!" << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "EXERCÍCIOS - 3D (REVISÃO)" << std::endl;
    std::cout << "Transformações Geométricas 2D e 3D" << std::endl;
    std::cout << "Rafael Bonfim - UFOP" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    
    problema1();
    problema2();
    
    std::cout << "Exercícios concluídos!" << std::endl;
    
    return 0;
}
