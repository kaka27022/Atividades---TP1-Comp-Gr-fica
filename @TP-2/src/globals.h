/*
 * globals.h
 * Variáveis globais e estruturas de dados - TP2 (3D)
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <string>
#include <cmath>

// ============================================================
// ESTRUTURAS DE DADOS
// ============================================================
struct Point3D {
    float x, y, z;
    Point3D() : x(0), y(0), z(0) {}
    Point3D(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Point3D operator+(const Point3D& p) const {
        return Point3D(x + p.x, y + p.y, z + p.z);
    }
    
    Point3D operator-(const Point3D& p) const {
        return Point3D(x - p.x, y - p.y, z - p.z);
    }
    
    Point3D operator*(float s) const {
        return Point3D(x * s, y * s, z * s);
    }
    
    float length() const {
        return sqrtf(x*x + y*y + z*z);
    }
    
    void normalize() {
        float len = length();
        if (len > 0.0001f) {
            x /= len;
            y /= len;
            z /= len;
        }
    }
};

struct Line3D {
    int p0, p1;  // índices dos pontos
    float radius;
    Line3D(int p0, int p1, float r = 0.5f) : p0(p0), p1(p1), radius(r) {}
};

// Estrutura para câmera
struct Camera {
    float distance;      // Distância do centro
    float azimuth;       // Ângulo horizontal (graus)
    float elevation;     // Ângulo vertical (graus)
    Point3D center;      // Centro de rotação
    Point3D eye;         // Posição do observador
    Point3D up;          // Vetor up
    
    Camera() : distance(10.0f), azimuth(45.0f), elevation(30.0f), 
               center(0, 0, 0), up(0, 1, 0) {
        updateEye();
    }
    
    void updateEye() {
        float rad_az = azimuth * 3.14159265358979323846f / 180.0f;
        float rad_el = elevation * 3.14159265358979323846f / 180.0f;
        
        eye.x = center.x + distance * cosf(rad_el) * cosf(rad_az);
        eye.y = center.y + distance * sinf(rad_el);
        eye.z = center.z + distance * cosf(rad_el) * sinf(rad_az);
    }
};

// Estrutura para iluminação
struct Light {
    Point3D position;
    float ambient[4];
    float diffuse[4];
    float specular[4];
    
    Light() {
        position = Point3D(8.0f, 10.0f, 8.0f);  // Posição mais alta e lateral para brilhos similares à imagem
        ambient[0] = 0.15f; ambient[1] = 0.15f; ambient[2] = 0.15f; ambient[3] = 1.0f;
        diffuse[0] = 0.9f; diffuse[1] = 0.9f; diffuse[2] = 0.9f; diffuse[3] = 1.0f;
        specular[0] = 1.0f; specular[1] = 1.0f; specular[2] = 1.0f; specular[3] = 1.0f;
    }
};

// ============================================================
// VARIÁVEIS GLOBAIS
// ============================================================

extern std::vector<Point3D> points;      // Pontos da árvore
extern std::vector<Line3D> lines;        // Segmentos da árvore
extern std::vector<float> radii;         // Raios dos segmentos

// Câmera
extern Camera camera;

// Iluminação
extern Light light;
extern float material_ambient[4];
extern float material_diffuse[4];
extern float material_specular[4];
extern float material_shininess;

// Modo de iluminação (0=Flat, 1=Phong)
extern int lighting_mode;
extern bool lighting_enabled;

// Transparência
extern bool transparency_enabled;
extern float transparency_alpha;

// Visualização incremental - arquivos parciais
extern std::vector<std::string> growth_files;
extern int current_growth_index;
extern bool growth_mode;

// Visualização incremental - segmentos
extern int n_segments_draw;
extern int max_segments;

// Animação
extern bool animation_enabled;
extern float animation_speed;
extern float animation_timer;

// Seleção de segmento
extern int selected_segment;
extern bool show_segment_info;

// Dimensões da janela
extern int window_width;
extern int window_height;

// Qualidade da renderização de cilindros
extern int cylinder_quality;  // Número de lados do cilindro

// Escala dos dados (calculada uma vez ao carregar)
extern float data_scale;  // Escala máxima dos dados (para normalização visual)

// Modo de raio (0=fixo, 1=variável)
extern bool radius_mode_fixed;  // true = raio fixo, false = raio variável

#endif // GLOBALS_H
