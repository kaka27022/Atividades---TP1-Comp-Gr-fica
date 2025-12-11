/*
 * globals.cpp
 * Definição das variáveis globais
 */

#include "globals.h"

// ============================================================
// VARIÁVEIS GLOBAIS
// ============================================================

std::vector<Point2D> points;      // Pontos da árvore
std::vector<Line> lines;           // Segmentos da árvore
std::vector<float> radii;          // Raios dos segmentos

// Parâmetros de transformação
float tx = 0.0f, ty = 0.0f;        // Translação
float angle = 0.0f;                // Rotação (em graus)
float scale = 12.0f;   
float line_thickness_factor = 1.0f;   // multiplicador da espessura
int thickness_level = 1;              // 0=fino, 1=médio, 2=grosso


// Visualização incremental - arquivos parciais
std::vector<std::string> growth_files;  // Lista de arquivos de crescimento
int current_growth_index = 0;            // Índice do arquivo atual
bool growth_mode = false;                // Modo de crescimento incremental

// Visualização incremental - segmentos
int n_segments_draw = 0;           // Número de segmentos a desenhar
int max_segments = 0;

// Recorte (Cohen-Sutherland)
bool clip_enabled = false;
float clip_xmin, clip_xmax, clip_ymin, clip_ymax;
float clip_size = 0.3f;  // Tamanho da janela de recorte (fração da viewport)

// Dimensões da janela
int window_width = 800;
int window_height = 600;



