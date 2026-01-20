/*
 * interface.cpp
 * Implementação das funções de interface gráfica - TP2 (3D)
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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
// FUNÇÕES DE ILUMINAÇÃO
// ============================================================

void setupLightingFlat(const Point3D& normal, float base_r, float base_g, float base_b) {
    // Iluminação Flat - calcula apenas uma cor por face usando cores baseadas no raio
    Point3D lightDir = light.position;
    lightDir.normalize();
    
    float ndotl = dotProduct(normal, lightDir);
    if (ndotl < 0.0f) ndotl = 0.0f;
    
    float ambient_intensity = 0.3f;
    float r = base_r * ambient_intensity + base_r * ndotl * 0.9f;
    float g = base_g * ambient_intensity + base_g * ndotl * 0.9f;
    float b = base_b * ambient_intensity + base_b * ndotl * 0.9f;
    
    r = std::min(1.0f, std::max(0.0f, r));
    g = std::min(1.0f, std::max(0.0f, g));
    b = std::min(1.0f, std::max(0.0f, b));
    
    glColor3f(r, g, b);
}

void setupLightingFlat(const Point3D& normal) {
    // Versão sem cores customizadas
    Point3D lightDir = light.position;
    lightDir.normalize();
    
    float ndotl = dotProduct(normal, lightDir);
    if (ndotl < 0.0f) ndotl = 0.0f;
    
    float r = material_ambient[0] + material_diffuse[0] * ndotl;
    float g = material_ambient[1] + material_diffuse[1] * ndotl;
    float b = material_ambient[2] + material_diffuse[2] * ndotl;
    
    r = std::min(1.0f, std::max(0.0f, r));
    g = std::min(1.0f, std::max(0.0f, g));
    b = std::min(1.0f, std::max(0.0f, b));
    
    glColor3f(r, g, b);
}

void getColorFromRadius(float normalized_radius, float& r, float& g, float& b) {
    // Gradiente de azul (raios pequenos) para vermelho (raios grandes)
    // Similar à barra de cores das imagens de referência
    float t = normalized_radius;
    
    if (t < 0.1667f) {
        // Azul escuro para azul claro
        float local_t = t / 0.1667f;
        r = 0.1f + local_t * 0.1f;
        g = 0.2f + local_t * 0.3f;
        b = 0.6f + local_t * 0.4f;
    } else if (t < 0.3333f) {
        // Azul claro para ciano
        float local_t = (t - 0.1667f) / 0.1667f;
        r = 0.2f;
        g = 0.5f + local_t * 0.3f;
        b = 1.0f;
    } else if (t < 0.5f) {
        // Ciano para verde
        float local_t = (t - 0.3333f) / 0.1667f;
        r = 0.2f;
        g = 0.8f + local_t * 0.2f;
        b = 1.0f - local_t * 0.8f;
    } else if (t < 0.6667f) {
        // Verde para amarelo
        float local_t = (t - 0.5f) / 0.1667f;
        r = 0.2f + local_t * 0.8f;
        g = 1.0f;
        b = 0.2f - local_t * 0.2f;
    } else if (t < 0.8333f) {
        // Amarelo para laranja
        float local_t = (t - 0.6667f) / 0.1667f;
        r = 1.0f;
        g = 1.0f - local_t * 0.3f;
        b = 0.0f;
    } else {
        // Laranja para vermelho
        float local_t = (t - 0.8333f) / 0.1667f;
        r = 1.0f;
        g = 0.7f - local_t * 0.4f;
        b = 0.0f;
    }
}

void setupLightingPhong(const Point3D& vertex, const Point3D& normal, const Point3D& eye, 
                         float base_r, float base_g, float base_b) {
    // Iluminação Phong - calcula cor considerando ambiente, difuso e especular
    // Usa cores baseadas no raio do segmento
    Point3D lightDir = light.position - vertex;
    lightDir.normalize();
    
    Point3D viewDir = eye - vertex;
    viewDir.normalize();
    
    // Componente ambiente (usar cor base com intensidade reduzida)
    float ambient_intensity = 0.3f;
    float r = base_r * ambient_intensity;
    float g = base_g * ambient_intensity;
    float b = base_b * ambient_intensity;
    
    // Componente difuso
    float ndotl = dotProduct(normal, lightDir);
    if (ndotl < 0.0f) ndotl = 0.0f;
    
    r += base_r * light.diffuse[0] * ndotl * 0.9f;
    g += base_g * light.diffuse[1] * ndotl * 0.9f;
    b += base_b * light.diffuse[2] * ndotl * 0.9f;
    
    // Componente especular (Phong) - sempre branco para reflexos
    if (ndotl > 0.0f) {
        Point3D reflectDir = normal * (2.0f * ndotl) - lightDir;
        reflectDir.normalize();
        float rdotv = dotProduct(reflectDir, viewDir);
        if (rdotv < 0.0f) rdotv = 0.0f;
        
        float specular = powf(rdotv, material_shininess);
        float spec_intensity = 0.8f * specular;
        r += spec_intensity;
        g += spec_intensity;
        b += spec_intensity;
    }
    
    r = std::min(1.0f, std::max(0.0f, r));
    g = std::min(1.0f, std::max(0.0f, g));
    b = std::min(1.0f, std::max(0.0f, b));
    
    if (transparency_enabled) {
        glColor4f(r, g, b, transparency_alpha);
    } else {
        glColor3f(r, g, b);
    }
}

void setupLightingPhong(const Point3D& vertex, const Point3D& normal, const Point3D& eye) {
    // Versão sem cores customizadas - usar cores do material padrão
    Point3D lightDir = light.position - vertex;
    lightDir.normalize();
    
    Point3D viewDir = eye - vertex;
    viewDir.normalize();
    
    float ambient_intensity = 0.3f;
    float r = material_ambient[0] * ambient_intensity;
    float g = material_ambient[1] * ambient_intensity;
    float b = material_ambient[2] * ambient_intensity;
    
    float ndotl = dotProduct(normal, lightDir);
    if (ndotl < 0.0f) ndotl = 0.0f;
    
    r += material_diffuse[0] * light.diffuse[0] * ndotl;
    g += material_diffuse[1] * light.diffuse[1] * ndotl;
    b += material_diffuse[2] * light.diffuse[2] * ndotl;
    
    if (ndotl > 0.0f) {
        Point3D reflectDir = normal * (2.0f * ndotl) - lightDir;
        reflectDir.normalize();
        float rdotv = dotProduct(reflectDir, viewDir);
        if (rdotv < 0.0f) rdotv = 0.0f;
        
        float specular = powf(rdotv, material_shininess);
        r += material_specular[0] * light.specular[0] * specular;
        g += material_specular[1] * light.specular[1] * specular;
        b += material_specular[2] * light.specular[2] * specular;
    }
    
    r = std::min(1.0f, std::max(0.0f, r));
    g = std::min(1.0f, std::max(0.0f, g));
    b = std::min(1.0f, std::max(0.0f, b));
    
    if (transparency_enabled) {
        glColor4f(r, g, b, transparency_alpha);
    } else {
        glColor3f(r, g, b);
    }
}

// ============================================================
// DESENHO DE CILINDRO 3D
// ============================================================

void drawCylinder(const Point3D& p0, const Point3D& p1, float radius, int segments,
                  float base_r, float base_g, float base_b) {
    bool use_custom_color = (base_r >= 0.0f && base_g >= 0.0f && base_b >= 0.0f);
    if (segments < 3) segments = 3;
    
    Point3D dir = p1 - p0;
    float length = dir.length();
    if (length < 0.0001f) return;
    
    dir.normalize();
    
    // Encontrar dois vetores perpendiculares para criar a base do cilindro
    Point3D up(0, 1, 0);
    if (fabsf(dotProduct(dir, up)) > 0.9f) {
        up = Point3D(1, 0, 0);
    }
    
    Point3D u = crossProduct(up, dir);
    u.normalize();
    Point3D v = crossProduct(dir, u);
    v.normalize();
    
    // Gerar vértices das bases
    std::vector<Point3D> base0(segments);
    std::vector<Point3D> base1(segments);
    
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float cos_a = cosf(angle);
        float sin_a = sinf(angle);
        
        Point3D offset = (u * cos_a + v * sin_a) * radius;
        base0[i] = p0 + offset;
        base1[i] = p1 + offset;
    }
    
    // Desenhar superfície lateral
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        int idx = i % segments;
        Point3D normal = base0[idx] - p0;
        normal.normalize();
        
        Point3D vertex0 = base0[idx];
        Point3D vertex1 = base1[idx];
        
        if (lighting_enabled) {
            if (lighting_mode == 0) {
                if (use_custom_color) {
                    setupLightingFlat(normal, base_r, base_g, base_b);
                } else {
                    setupLightingFlat(normal);
                }
            } else {
                if (use_custom_color) {
                    setupLightingPhong(vertex0, normal, camera.eye, base_r, base_g, base_b);
                } else {
                    setupLightingPhong(vertex0, normal, camera.eye);
                }
            }
        } else if (use_custom_color) {
            if (transparency_enabled) {
                glColor4f(base_r, base_g, base_b, transparency_alpha);
            } else {
                glColor3f(base_r, base_g, base_b);
            }
        }
        
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(vertex0.x, vertex0.y, vertex0.z);
        
        if (lighting_enabled && lighting_mode == 1) {
            if (use_custom_color) {
                setupLightingPhong(vertex1, normal, camera.eye, base_r, base_g, base_b);
            } else {
                setupLightingPhong(vertex1, normal, camera.eye);
            }
        } else if (use_custom_color) {
            if (transparency_enabled) {
                glColor4f(base_r, base_g, base_b, transparency_alpha);
            } else {
                glColor3f(base_r, base_g, base_b);
            }
        }
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(vertex1.x, vertex1.y, vertex1.z);
    }
    glEnd();
    
    // Desenhar bases (tampas)
    // Base 0
    Point3D normal0 = dir * -1.0f;
    glBegin(GL_TRIANGLE_FAN);
    if (lighting_enabled) {
        if (lighting_mode == 0) {
            if (use_custom_color) {
                setupLightingFlat(normal0, base_r, base_g, base_b);
            } else {
                setupLightingFlat(normal0);
            }
        } else {
            if (use_custom_color) {
                setupLightingPhong(p0, normal0, camera.eye, base_r, base_g, base_b);
            } else {
                setupLightingPhong(p0, normal0, camera.eye);
            }
        }
    } else if (use_custom_color) {
        if (transparency_enabled) {
            glColor4f(base_r, base_g, base_b, transparency_alpha);
        } else {
            glColor3f(base_r, base_g, base_b);
        }
    }
    glNormal3f(normal0.x, normal0.y, normal0.z);
    glVertex3f(p0.x, p0.y, p0.z);
    
    for (int i = 0; i <= segments; i++) {
        int idx = i % segments;
        if (lighting_enabled && lighting_mode == 1) {
            if (use_custom_color) {
                setupLightingPhong(base0[idx], normal0, camera.eye, base_r, base_g, base_b);
            } else {
                setupLightingPhong(base0[idx], normal0, camera.eye);
            }
        } else if (use_custom_color) {
            if (transparency_enabled) {
                glColor4f(base_r, base_g, base_b, transparency_alpha);
            } else {
                glColor3f(base_r, base_g, base_b);
            }
        }
        glNormal3f(normal0.x, normal0.y, normal0.z);
        glVertex3f(base0[idx].x, base0[idx].y, base0[idx].z);
    }
    glEnd();
    
    // Base 1
    Point3D normal1 = dir;
    glBegin(GL_TRIANGLE_FAN);
    if (lighting_enabled) {
        if (lighting_mode == 0) {
            if (use_custom_color) {
                setupLightingFlat(normal1, base_r, base_g, base_b);
            } else {
                setupLightingFlat(normal1);
            }
        } else {
            if (use_custom_color) {
                setupLightingPhong(p1, normal1, camera.eye, base_r, base_g, base_b);
            } else {
                setupLightingPhong(p1, normal1, camera.eye);
            }
        }
    } else if (use_custom_color) {
        if (transparency_enabled) {
            glColor4f(base_r, base_g, base_b, transparency_alpha);
        } else {
            glColor3f(base_r, base_g, base_b);
        }
    }
    glNormal3f(normal1.x, normal1.y, normal1.z);
    glVertex3f(p1.x, p1.y, p1.z);
    
    for (int i = segments; i >= 0; i--) {
        int idx = i % segments;
        if (lighting_enabled && lighting_mode == 1) {
            if (use_custom_color) {
                setupLightingPhong(base1[idx], normal1, camera.eye, base_r, base_g, base_b);
            } else {
                setupLightingPhong(base1[idx], normal1, camera.eye);
            }
        } else if (use_custom_color) {
            if (transparency_enabled) {
                glColor4f(base_r, base_g, base_b, transparency_alpha);
            } else {
                glColor3f(base_r, base_g, base_b);
            }
        }
        glNormal3f(normal1.x, normal1.y, normal1.z);
        glVertex3f(base1[idx].x, base1[idx].y, base1[idx].z);
    }
    glEnd();
}

// ============================================================
// DESENHO DA ÁRVORE 3D
// ============================================================

void drawTree3D() {
    if (points.empty() || lines.empty()) return;
    
    // Configurar transparência
    if (transparency_enabled) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);  // Permitir que objetos transparentes sejam desenhados corretamente
    } else {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);   // Habilitar write no depth buffer quando não transparente
    }
    
    // Normalizar raios para visualização (para o gradiente de cores)
    float min_r = 1e9f, max_r = -1e9f;
    for (const auto& L : lines) {
        if (L.radius < min_r) min_r = L.radius;
        if (L.radius > max_r) max_r = L.radius;
    }
    
    float range_r = max_r - min_r;
    if (range_r < 1e-6f) range_r = 1.0f;
    
    // Calcular raio médio para escala
    float avg_radius = (min_r + max_r) / 2.0f;
    
    // Desenhar segmentos como cilindros
    int count = 0;
    
    // Calcular raio médio para modo fixo
    float fixed_radius = 0.0f;
    if (radius_mode_fixed && avg_radius > 0.0001f) {
        fixed_radius = avg_radius;
    }
    
    for (size_t i = 0; i < lines.size() && count < n_segments_draw; i++) {
        Point3D p0 = points[lines[i].p0];
        Point3D p1 = points[lines[i].p1];
        
        // Escolher entre raio fixo ou variável
        float segment_radius;
        if (radius_mode_fixed && fixed_radius > 0.0f) {
            // Modo (a): raio fixo - usar o raio médio para todos os segmentos
            segment_radius = fixed_radius;
        } else {
            // Modo (b): raio variável - usar o raio original do arquivo VTK
            segment_radius = lines[i].radius;
        }
        
        // Escala dos raios para corresponder aos exemplos Nterm:
        // - Modo FIXO: todos os segmentos com a mesma espessura (raio médio), tubos com espessura uniforme
        // - Modo VARIÁVEL: espessura proporcional ao raio do VTK (tronco grosso, ramos finos)
        // Referência: raio máximo do tronco em ~2.5% do data_scale (tubos mais finos)
        
        float display_radius = segment_radius;
        
        if (data_scale > 0.001f) {
            float ref_r = (max_r > 0.0001f) ? max_r : avg_radius;
            if (ref_r > 0.0001f) {
                // Fator para que o raio MÁXIMO fique em ~2.5% do tamanho da cena
                float scale_factor = (data_scale * 0.025f) / ref_r;
                display_radius = segment_radius * scale_factor;
            }
        }
        
        // Limites de segurança (tubos visíveis, sem extremos)
        float min_radius = data_scale * 0.0015f;  // Mínimo: 0.15% do tamanho
        float max_radius = data_scale * 0.04f;    // Máximo: 4% do tamanho (preserva proporção tronco/ramos)
        
        if (display_radius < min_radius) display_radius = min_radius;
        if (display_radius > max_radius) display_radius = max_radius;
        
        // Normalizar raio para gradiente de cores (0.0 a 1.0)
        // Usar o raio original do arquivo VTK para as cores, mesmo no modo fixo
        float normalized_radius = (lines[i].radius - min_r) / range_r;
        
        // Calcular cor baseada no raio (gradiente de azul para vermelho)
        float base_r, base_g, base_b;
        getColorFromRadius(normalized_radius, base_r, base_g, base_b);
        
        // SEMPRE usar cores do gradiente, mesmo com iluminação
        // A iluminação será aplicada manualmente usando essas cores como base
        drawCylinder(p0, p1, display_radius, cylinder_quality, base_r, base_g, base_b);
        
        count++;
    }
    
    // Restaurar estado do depth buffer se estava desabilitado
    if (transparency_enabled) {
        glDepthMask(GL_TRUE);  // Restaurar write no depth buffer
        // NÃO desabilitar GL_BLEND aqui - será usado se houver outros objetos transparentes
    }
}

void drawSelectedSegment() {
    if (selected_segment < 0 || selected_segment >= (int)lines.size()) return;
    
    Line3D& line = lines[selected_segment];
    Point3D p0 = points[line.p0];
    Point3D p1 = points[line.p1];
    
    // Desenhar segmento selecionado com cor destacada
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);  // Amarelo
    glLineWidth(3.0f);
    
    glBegin(GL_LINES);
    glVertex3f(p0.x, p0.y, p0.z);
    glVertex3f(p1.x, p1.y, p1.z);
    glEnd();
    
    glLineWidth(1.0f);
    if (lighting_enabled) {
        glEnable(GL_LIGHTING);
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
    
    std::string lighting_mode_str = (lighting_mode == 0) ? "Flat" : "Phong";
    std::string radius_mode_str = radius_mode_fixed ? "FIXO" : "VARIÁVEL";
    std::string status = "Iluminação: " + std::string(lighting_enabled ? "ON" : "OFF") + 
                        " (" + lighting_mode_str + ") | " +
                        "Raio: " + radius_mode_str + " | " +
                        "Transparência: " + std::string(transparency_enabled ? "ON" : "OFF") + " | " +
                        "Segmentos: " + std::to_string(n_segments_draw) + "/" + std::to_string(max_segments) +
                        " | Câmera: dist=" + std::to_string(camera.distance).substr(0, 4) +
                        " az=" + std::to_string(camera.azimuth).substr(0, 5) + "°" +
                        " el=" + std::to_string(camera.elevation).substr(0, 5) + "°";
    
    if (selected_segment >= 0) {
        status += " | Selecionado: " + std::to_string(selected_segment);
        if (show_segment_info) {
            Line3D& line = lines[selected_segment];
            Point3D p0 = points[line.p0];
            Point3D p1 = points[line.p1];
            Point3D dir = p1 - p0;
            float length = dir.length();
            status += " (raio=" + std::to_string(line.radius).substr(0, 4) +
                     " comprimento=" + std::to_string(length).substr(0, 4) + ")";
        }
    }
    
    // Informação de crescimento incremental
    if (growth_mode && growth_files.size() > 1) {
        status += " | Crescimento: " + std::to_string(current_growth_index + 1) + 
                "/" + std::to_string(growth_files.size());
    }
    
    if (animation_enabled) {
        status += " | Animação: ON";
    }
    
    glRasterPos2f(10, window_height - 20);
    for (char c : status) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    
    std::string controls = "Controles: Mouse(arrastar=câmera) W/S(zoom) Q/E(azimuth) A/D(elevação) I(i=Flat/Phong) R(raio fixo/variável) T(transp) [](crescimento) M(animação)";
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Configurar câmera
    updateCamera();
    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z,
              camera.center.x, camera.center.y, camera.center.z,
              camera.up.x, camera.up.y, camera.up.z);
    
    // Desabilitar iluminação do OpenGL - vamos calcular manualmente
    // para poder usar cores customizadas baseadas no gradiente de raios
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    
    // Habilitar color tracking para usar glColor
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Desenhar árvore
    drawTree3D();
    
    // Desenhar segmento selecionado
    if (selected_segment >= 0) {
        drawSelectedSegment();
    }
    
    // Desenhar texto
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
    
    // Projeção perspectiva
    float aspect = (float)w / (float)h;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
}

void updateCamera() {
    camera.updateEye();
}

void updateAnimation(int /* value */) {
    if (animation_enabled) {
        animation_timer += animation_speed * 0.1f;
        
        // Atualizar índice de crescimento baseado no timer
        if (!growth_files.empty()) {
            float segment_time = 1.0f;  // Tempo por arquivo em segundos
            int frame = (int)(animation_timer / segment_time);
            current_growth_index = frame % growth_files.size();
            loadCurrentGrowthFile();
        }
        
        glutPostRedisplay();
        glutTimerFunc(50, updateAnimation, 0);  // ~20 FPS
    }
}

// ============================================================
// INICIALIZAÇÃO
// ============================================================

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Habilitar Z-buffer (remoção de superfícies escondidas)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Configurações de qualidade
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    
    std::cout << "\n";
    std::cout << "===========================================\n";
    std::cout << "   TP2 - Visualizador 3D Árvores Arteriais\n";
    std::cout << "===========================================\n";
    std::cout << "\n";
    std::cout << "CONTROLES:\n";
    std::cout << "  Mouse Arrastar - Rotacionar câmera (azimuth/elevation)\n";
    std::cout << "  W / S          - Zoom in/out\n";
    std::cout << "  Q / E          - Rotação horizontal (azimuth)\n";
    std::cout << "  A / D          - Rotação vertical (elevation)\n";
    std::cout << "  I              - Alternar modo de iluminação (Flat/Phong)\n";
    std::cout << "  L              - Toggle iluminação ON/OFF\n";
    std::cout << "  R              - Alternar modo de raio (Fixo/Variável)\n";
    std::cout << "  T              - Toggle transparência\n";
    std::cout << "  [/]            - Arquivo anterior/próximo de crescimento\n";
    std::cout << "  PageUp/Down    - Segmentos incrementais\n";
    std::cout << "  M              - Toggle animação do crescimento\n";
    std::cout << "  Click          - Selecionar segmento (ao implementar)\n";
    std::cout << "  ESPAÇO         - Reset câmera\n";
    std::cout << "  ESC            - Sair\n";
    std::cout << "\n";
    std::cout << "===========================================\n";
    std::cout << "\n";
}
