/*
 * globals.h
 * Variáveis globais e estruturas de dados
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <string>

// ============================================================
// ESTRUTURAS DE DADOS
// ============================================================
struct Point2D {
    float x, y;
    Point2D() : x(0), y(0) {}
    Point2D(float x, float y) : x(x), y(y) {}
};

struct Line {
    int p0, p1;  // índices dos pontos
    float radius;
    Line(int p0, int p1, float r = 0.5f) : p0(p0), p1(p1), radius(r) {}
};

// ============================================================
// VARIÁVEIS GLOBAIS
// ============================================================

extern std::vector<Point2D> points;      // Pontos da árvore
extern std::vector<Line> lines;           // Segmentos da árvore
extern std::vector<float> radii;        // Raios dos segmentos

// Parâmetros de transformação
extern float tx, ty;                     // Translação
extern float angle;                      // Rotação (em graus)
extern float scale;    
extern float line_thickness_factor;   
extern int thickness_level;
                

// Visualização incremental - arquivos parciais
extern std::vector<std::string> growth_files;  // Lista de arquivos de crescimento
extern int current_growth_index;                // Índice do arquivo atual
extern bool growth_mode;                        // Modo de crescimento incremental

// Visualização incremental - segmentos
extern int n_segments_draw;              // Número de segmentos a desenhar
extern int max_segments;                 // Número máximo de segmentos

// Recorte (Cohen-Sutherland)
extern bool clip_enabled;
extern float clip_xmin, clip_xmax, clip_ymin, clip_ymax;
extern float clip_size;                  // Tamanho da janela de recorte (fração da viewport)

// Dimensões da janela
extern int window_width;
extern int window_height;

#endif // GLOBALS_H

