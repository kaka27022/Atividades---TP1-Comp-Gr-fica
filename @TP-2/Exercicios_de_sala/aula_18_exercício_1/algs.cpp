#include "algs.h"

// Função auxiliar para produto escalar
float dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

// Implementação do Algoritmo Cyrus-Beck
bool cyrusBeckClip(Point p0, Point p1, const std::vector<Edge>& edges, Point &outP0, Point &outP1) {
    // 1. Inicialização dos parâmetros
    float tE = 0.0f; // Máximo limite inferior (Entrada)
    float tL = 1.0f; // Mínimo limite superior (Saída)

    // 2. Vetor Direção da reta D = P1 - P0
    Point D = { p1.x - p0.x, p1.y - p0.y };

    // 3. Processar cada aresta do polígono
    for (const auto& edge : edges) {
        // Vetor do ponto na aresta até o ponto inicial da reta: (Pe - P0)
        // Isso é o vetor que conecta P0 ao ponto da aresta
        Point diff = { edge.pE.x - p0.x, edge.pE.y - p0.y };
        
        // Numerador = N . (Pe - P0)
        float numerator = dot(edge.N, diff);
        // Denominador = N . D
        float denominator = dot(edge.N, D);

        // Caso: Reta Paralela à Aresta
        if (std::abs(denominator) < 1e-6) { 
            // Com Normal INTERNA:
            // Se numerator > 0, significa que P0 está do lado de FORA.
            // (Matematicamente: N.(Pe-P0) > 0 implica N.(P0-Pe) < 0, ou seja, oposto à normal)
            if (numerator > 0) return false; 
        } 
        else {
            float t = numerator / denominator;

            if (denominator > 0) {
                // CORREÇÃO CRÍTICA AQUI:
                // Com Normal Apontando para DENTRO:
                // Se (N . D) > 0, o ângulo é agudo. A reta está indo na direção da normal.
                // Isso significa que ela está ENTRANDO no polígono.
                tE = std::max(tE, t);
            } else {
                // Se (N . D) < 0, o ângulo é obtuso. A reta está indo contra a normal.
                // Isso significa que ela está SAINDO do polígono.
                tL = std::min(tL, t);
            }
        }
    }

    // 4. Verificação final de visibilidade
    if (tE <= tL) {
        outP0.x = p0.x + tE * D.x;
        outP0.y = p0.y + tE * D.y;
        
        outP1.x = p0.x + tL * D.x;
        outP1.y = p0.y + tL * D.y;
        return true;
    }

    return false;
}