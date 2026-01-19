/*
 * globals.cpp
 * Definição das variáveis globais - TP2 (3D)
 */

#include "globals.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
// VARIÁVEIS GLOBAIS
// ============================================================

std::vector<Point3D> points;
std::vector<Line3D> lines;
std::vector<float> radii;

// Câmera
Camera camera;

// Iluminação
Light light;
// Cores azul-arroxeadas similares à imagem (periwinkle/lavender-blue)
float material_ambient[4] = {0.15f, 0.15f, 0.35f, 1.0f};  // Azul-roxo escuro
float material_diffuse[4] = {0.4f, 0.35f, 0.6f, 1.0f};    // Azul-arroxeado médio
float material_specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};    // Branco para reflexos
float material_shininess = 80.0f;  // Mais brilhante para reflexos brancos mais destacados

int lighting_mode = 1;  // 0=Flat, 1=Phong
bool lighting_enabled = true;

// Transparência
bool transparency_enabled = false;
float transparency_alpha = 0.7f;

// Visualização incremental
std::vector<std::string> growth_files;
int current_growth_index = 0;
bool growth_mode = false;

// Visualização incremental - segmentos
int n_segments_draw = 0;
int max_segments = 0;

// Animação
bool animation_enabled = false;
float animation_speed = 1.0f;
float animation_timer = 0.0f;

// Seleção
int selected_segment = -1;
bool show_segment_info = false;

// Dimensões da janela
int window_width = 800;
int window_height = 600;

// Qualidade dos cilindros
int cylinder_quality = 16;  // 16 lados para cilindros suaves (balance entre qualidade e performance)

// Escala dos dados
float data_scale = 1.0f;  // Será calculada ao carregar o arquivo

// Modo de raio (false = variável, true = fixo)
bool radius_mode_fixed = false;  // Por padrão usa raio variável
