/*
 * interface.cpp
 * Implementação das funções de interface gráfica
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "interface.h"
#include "globals.h"
#include "utils.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// ============================================================
// TRANSFORMAÇÕES GEOMÉTRICAS (Matrizes Homogêneas 3x3)
// ============================================================

#define M_PI 3.14159265358979323846

void applyTransform(Point2D& p) {
    // Matriz de transformação: T * R * S
    // Ordem: Escala -> Rotação -> Translação
    
    float rad = angle * M_PI / 180.0f;
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    
    // Aplicar escala
    float sx = p.x * scale;
    float sy = p.y * scale;
    
    // Aplicar rotação
    float rx = sx * cos_a - sy * sin_a;
    float ry = sx * sin_a + sy * cos_a;
    
    // Aplicar translação
    p.x = rx + tx;
    p.y = ry + ty;
}

// ============================================================
// RECORTE COHEN-SUTHERLAND
// ============================================================

OutCode computeOutCode(float x, float y) {
    OutCode code = INSIDE;
    if (x < clip_xmin) code = static_cast<OutCode>(code | LEFT);
    if (x > clip_xmax) code = static_cast<OutCode>(code | RIGHT);
    if (y < clip_ymin) code = static_cast<OutCode>(code | BOTTOM);
    if (y > clip_ymax) code = static_cast<OutCode>(code | TOP);
    return code;
}

bool cohenSutherlandClip(float& x0, float& y0, float& x1, float& y1) {
    OutCode outcode0 = computeOutCode(x0, y0);
    OutCode outcode1 = computeOutCode(x1, y1);
    
    bool accept = false;
    
    while (true) {
        if (!(outcode0 | outcode1)) {
            // Ambos os pontos estão dentro
            accept = true;
            break;
        } else if (outcode0 & outcode1) {
            // Ambos os pontos estão fora do mesmo lado
            break;
        } else {
            // Pelo menos um ponto está fora, calcular interseção
            float x, y;
            OutCode outcodeOut = outcode0 ? outcode0 : outcode1;
            
            // Encontrar ponto de interseção
            // Proteção contra divisão por zero
            if (outcodeOut & TOP) {
                float dy = y1 - y0;
                if (fabs(dy) > 1e-6) {
                    x = x0 + (x1 - x0) * (clip_ymax - y0) / dy;
                    y = clip_ymax;
                } else {
                    // Linha horizontal, usar coordenada x do ponto
                    x = x0;
                    y = clip_ymax;
                }
            } else if (outcodeOut & BOTTOM) {
                float dy = y1 - y0;
                if (fabs(dy) > 1e-6) {
                    x = x0 + (x1 - x0) * (clip_ymin - y0) / dy;
                    y = clip_ymin;
                } else {
                    x = x0;
                    y = clip_ymin;
                }
            } else if (outcodeOut & RIGHT) {
                float dx = x1 - x0;
                if (fabs(dx) > 1e-6) {
                    y = y0 + (y1 - y0) * (clip_xmax - x0) / dx;
                    x = clip_xmax;
                } else {
                    // Linha vertical, usar coordenada y do ponto
                    x = clip_xmax;
                    y = y0;
                }
            } else if (outcodeOut & LEFT) {
                float dx = x1 - x0;
                if (fabs(dx) > 1e-6) {
                    y = y0 + (y1 - y0) * (clip_xmin - x0) / dx;
                    x = clip_xmin;
                } else {
                    x = clip_xmin;
                    y = y0;
                }
            }
            
            // Substituir ponto fora pelo ponto de interseção
            if (outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = computeOutCode(x0, y0);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = computeOutCode(x1, y1);
            }
        }
    }
    
    return accept;
}

// ============================================================
// DESENHO
// ============================================================
void drawTree() {
    if (points.empty() || lines.empty()) return;

    // ===============================
    // NORMALIZAÇÃO AUTOMÁTICA DO RAIO
    // ===============================
    float min_r = 1e9f, max_r = -1e9f;

    for (const auto& L : lines) {
        if (L.radius < min_r) min_r = L.radius;
        if (L.radius > max_r) max_r = L.radius;
    }

    float range_r = max_r - min_r;
    if (range_r < 1e-6f) range_r = 1.0f;

    // ===============================
    // CÁLCULO DA JANELA DE RECORTE
    // ===============================
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble x0p, y0p, z0p, x1p, y1p, z1p, center_x, center_y, center_z;

    gluUnProject(0, 0, 0, modelview, projection, viewport, &x0p, &y0p, &z0p);
    gluUnProject(window_width, window_height, 0, modelview, projection, viewport, &x1p, &y1p, &z1p);
    gluUnProject(window_width / 2.0, window_height / 2.0, 0, modelview, projection, viewport,
                &center_x, &center_y, &center_z);

    float viewport_width  = std::abs(x1p - x0p);
    float viewport_height = std::abs(y1p - y0p);

    float clip_width  = viewport_width  * clip_size;
    float clip_height = viewport_height * clip_size;

    clip_xmin = center_x - clip_width  / 2.0f;
    clip_xmax = center_x + clip_width  / 2.0f;
    clip_ymin = center_y - clip_height / 2.0f;
    clip_ymax = center_y + clip_height / 2.0f;

    // ===============================
    // DESENHAR ÁRVORE
    // ===============================
    glBegin(GL_LINES);

    int count = 0;
    for (size_t i = 0; i < lines.size() && count < n_segments_draw; i++) {

        Point2D p0 = points[lines[i].p0];
        Point2D p1 = points[lines[i].p1];

        applyTransform(p0);
        applyTransform(p1);

        float x0 = p0.x, y0 = p0.y;
        float x1 = p1.x, y1 = p1.y;

        bool draw = true;
        if (clip_enabled)
            draw = cohenSutherlandClip(x0, y0, x1, y1);

        if (draw) {

            // ===========================
            // ESPESSURA DO TRAÇO (3 níveis)
            // ===========================
            float t_radius = (lines[i].radius - min_r) / range_r;
            float base_thick = 0.4f + t_radius * 2.0f;

            float multiplier =
                (thickness_level == 0 ? 0.6f :
                (thickness_level == 1 ? 1.0f :
                                        1.6f)); // 0=fino, 1=médio, 2=grosso

            float thickness = base_thick * multiplier;

            glEnd();
            glLineWidth(thickness);
            glBegin(GL_LINES);

            // ===========================
            // COR: gradiente por nível
            // ===========================
            float t = (lines[i].radius - min_r) / range_r;
            t = 1.0f - t; // menor raio = mais quente

            int level = std::min(4, int(t * 5.0f));
            float local_t = (t * 5.0f) - level;

            float c0_r, c0_g, c0_b;
            float c1_r, c1_g, c1_b;

            switch (level) {
            case 0: // vermelho → laranja
                c0_r = 1.0f; c0_g = 0.0f;  c0_b = 0.0f;
                c1_r = 1.0f; c1_g = 0.5f;  c1_b = 0.0f;
                break;
            case 1: // laranja → amarelo
                c0_r = 1.0f; c0_g = 0.5f;  c0_b = 0.0f;
                c1_r = 1.0f; c1_g = 1.0f;  c1_b = 0.0f;
                break;
            case 2: // amarelo → verde
                c0_r = 1.0f; c0_g = 1.0f;  c0_b = 0.0f;
                c1_r = 0.0f; c1_g = 1.0f;  c1_b = 0.0f;
                break;
            case 3: // verde → azul claro
                c0_r = 0.0f; c0_g = 1.0f;  c0_b = 0.0f;
                c1_r = 0.0f; c1_g = 0.8f;  c1_b = 1.0f;
                break;
            case 4: // azul claro → azul escuro
            default:
                c0_r = 0.0f; c0_g = 0.8f;  c0_b = 1.0f;
                c1_r = 0.0f; c1_g = 0.2f;  c1_b = 0.6f;
                break;
            }

            float r = c0_r * (1.0f - local_t) + c1_r * local_t;
            float g = c0_g * (1.0f - local_t) + c1_g * local_t;
            float b = c0_b * (1.0f - local_t) + c1_b * local_t;

            glColor3f(r, g, b);

            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
        }

        count++;
    }

    glEnd();

    // ===============================
    // DESENHAR JANELA DE RECORTE
    // ===============================
    if (clip_enabled) {
        glLineWidth(1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glLineStipple(1, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);

        glBegin(GL_LINE_LOOP);
        glVertex2f(clip_xmin, clip_ymin);
        glVertex2f(clip_xmax, clip_ymin);
        glVertex2f(clip_xmax, clip_ymax);
        glVertex2f(clip_xmin, clip_ymax);
        glEnd();

        glDisable(GL_LINE_STIPPLE);
    }
}

void displayText() {
    // Desenhar informações na tela
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, window_width, 0, window_height);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    std::string thickness_labels[] = {"fino", "médio", "grosso"};
    std::string status = "tx=" + std::to_string(tx).substr(0, 4) + 
                        " ty=" + std::to_string(ty).substr(0, 4) +
                        " rot=" + std::to_string(angle).substr(0, 5) + "°" +
                        " scale=" + std::to_string(scale).substr(0, 4) +
                        " segs=" + std::to_string(n_segments_draw) + "/" + std::to_string(max_segments) +
                        " thick=" + thickness_labels[thickness_level] +
                        " clip=" + (clip_enabled ? "ON" : "OFF");
    
    if (clip_enabled) {
        status += " size=" + std::to_string(clip_size * 100).substr(0, 3) + "%";
    }
    
    // Adicionar informação de crescimento incremental
    if (growth_mode && growth_files.size() > 1) {
        status += " | Crescimento: " + std::to_string(current_growth_index + 1) + 
                "/" + std::to_string(growth_files.size());
        if (!growth_files.empty()) {
            std::string current_file = getFilename(growth_files[current_growth_index]);
            status += " [" + current_file + "]";
        }
    }
    
    glRasterPos2f(10, window_height - 20);
    for (char c : status) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    
    std::string controls = "Controles: Setas(transl) r/R(rot) +/-(scale) t/T(thick) c(clip) x/z(clip size) [](growth) PageUp/Down(segs)";
    glRasterPos2f(10, window_height - 40);
    for (char c : controls) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drawTree();
    displayText();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    window_width = w;
    window_height = h;
    
    if (h == 0) h = 1;
    
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Projeção ortográfica 2D
    // Ajustar para manter aspect ratio
    float aspect = (float)w / (float)h;
    float view_size = 1.0f;
    
    if (aspect > 1.0f) {
        gluOrtho2D(-view_size * aspect, view_size * aspect, -view_size, view_size);
    } else {
        gluOrtho2D(-view_size, view_size, -view_size / aspect, view_size / aspect);
    }
    
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
// INICIALIZAÇÃO
// ============================================================
 
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Fundo preto
    glColor3f(1.0f, 1.0f, 1.0f);
    
    std::cout << "\n";
    std::cout << "===========================================\n";
    std::cout << "   TP1 - Visualizador 2D Árvores Arteriais\n";
    std::cout << "===========================================\n";
    std::cout << "\n";
    std::cout << "CONTROLES:\n";
    std::cout << "  Setas     - Translação (← → ↑ ↓)\n";
    std::cout << "  r/R       - Rotação (horário/anti-horário)\n";
    std::cout << "  +/-       - Escala (aumentar/diminuir)\n";
    std::cout << "  t/T       - Espessura das linhas (fino/médio/grosso)\n";
    std::cout << "  c         - Toggle recorte (clipping)\n";
    std::cout << "  x/z       - Aumentar/diminuir tamanho da janela de recorte\n";
    std::cout << "  [/]       - Arquivo anterior/próximo de crescimento\n";
    std::cout << "  PageUp/Dn - Segmentos incrementais (dentro do arquivo)\n";
    std::cout << "  ESPAÇO    - Reset transformações\n";
    std::cout << "  ESC       - Sair\n";
    std::cout << "\n";
    std::cout << "===========================================\n";
    std::cout << "\n";
}

