/*
 * handlers.cpp
 * Implementação dos handlers de eventos - TP2 (3D)
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "handlers.h"
#include "globals.h"
#include "interface.h"
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

// Variáveis para controle do mouse
static int last_mouse_x = 0;
static int last_mouse_y = 0;
static bool mouse_left_pressed = false;

void keyboard(unsigned char key, int, int) {
    float step = 5.0f;
    
    // Debug: verificar se a função está sendo chamada (apenas para teclas importantes)
    // std::cout << "Tecla pressionada: '" << key << "' (ASCII: " << (int)key << ")" << std::endl;
    
    switch (key) {
        case 'w':
        case 'W':
            // Zoom in (decremento de 10% da distância atual)
            std::cout << ">>> W pressionado - Distância ANTES: " << camera.distance << std::endl;
            camera.distance = std::max(0.05f, camera.distance * 0.9f);  // Reduzir 10%
            camera.updateEye();
            std::cout << ">>> W pressionado - Distância DEPOIS: " << camera.distance << std::endl;
            break;
        case 's':
        case 'S':
            // Zoom out (incremento de 10% da distância atual)
            std::cout << ">>> S pressionado - Distância ANTES: " << camera.distance << std::endl;
            camera.distance *= 1.1f;  // Aumentar 10%
            camera.updateEye();
            std::cout << ">>> S pressionado - Distância DEPOIS: " << camera.distance << std::endl;
            break;
        case 'q':
        case 'Q':
            // Rotação horizontal (azimuth) -
            camera.azimuth -= step;
            camera.updateEye();
            break;
        case 'e':
        case 'E':
            // Rotação horizontal (azimuth) +
            camera.azimuth += step;
            camera.updateEye();
            break;
        case 'a':
        case 'A':
            // Rotação vertical (elevation) -
            camera.elevation = std::max(-89.0f, camera.elevation - step);
            camera.updateEye();
            break;
        case 'd':
        case 'D':
            // Rotação vertical (elevation) +
            camera.elevation = std::min(89.0f, camera.elevation + step);
            camera.updateEye();
            break;
        case 'i':
        case 'I':
            // Alternar modo de iluminação (Flat/Phong)
            lighting_mode = (lighting_mode + 1) % 2;
            std::cout << "Modo de iluminação: " << (lighting_mode == 0 ? "Flat" : "Phong") << std::endl;
            break;
        case 'l':
        case 'L':
            // Toggle iluminação
            lighting_enabled = !lighting_enabled;
            std::cout << "Iluminação: " << (lighting_enabled ? "ON" : "OFF") << std::endl;
            break;
        case 't':
        case 'T':
            // Toggle transparência
            transparency_enabled = !transparency_enabled;
            std::cout << "Transparência: " << (transparency_enabled ? "ON" : "OFF") << std::endl;
            break;
        case '[':
            // Arquivo anterior de crescimento
            if (!growth_files.empty() && growth_files.size() > 1) {
                current_growth_index--;
                if (current_growth_index < 0) {
                    current_growth_index = growth_files.size() - 1;
                }
                // Forçar modo de crescimento ao carregar
                bool prev_growth_mode = growth_mode;
                growth_mode = true;
                if (loadCurrentGrowthFile()) {
                    std::cout << "Arquivo anterior: " << (current_growth_index + 1) << "/" << growth_files.size() << std::endl;
                    // Garantir que estamos mostrando todos os segmentos do novo arquivo
                    n_segments_draw = max_segments;
                } else {
                    std::cerr << "Erro ao carregar arquivo de crescimento" << std::endl;
                    growth_mode = prev_growth_mode;
                }
            } else {
                std::cout << "Nenhum arquivo de crescimento disponível" << std::endl;
            }
            break;
        case ']':
            // Próximo arquivo de crescimento
            if (!growth_files.empty() && growth_files.size() > 1) {
                current_growth_index++;
                if (current_growth_index >= (int)growth_files.size()) {
                    current_growth_index = 0;
                }
                // Forçar modo de crescimento ao carregar
                bool prev_growth_mode = growth_mode;
                growth_mode = true;
                if (loadCurrentGrowthFile()) {
                    std::cout << "Próximo arquivo: " << (current_growth_index + 1) << "/" << growth_files.size() << std::endl;
                    // Garantir que estamos mostrando todos os segmentos do novo arquivo
                    n_segments_draw = max_segments;
                } else {
                    std::cerr << "Erro ao carregar arquivo de crescimento" << std::endl;
                    growth_mode = prev_growth_mode;
                }
            } else {
                std::cout << "Nenhum arquivo de crescimento disponível" << std::endl;
            }
            break;
        case 'm':
        case 'M':
            // Toggle animação
            animation_enabled = !animation_enabled;
            if (animation_enabled) {
                animation_timer = 0.0f;
                updateAnimation(0);
                std::cout << "Animação: ON" << std::endl;
            } else {
                std::cout << "Animação: OFF" << std::endl;
            }
            break;
        case 'r':
        case 'R':
            // Toggle modo de raio (fixo/variável)
            radius_mode_fixed = !radius_mode_fixed;
            std::cout << "Modo de raio: " << (radius_mode_fixed ? "FIXO" : "VARIÁVEL") << std::endl;
            break;
        case ' ':
            // Reset câmera
            camera.distance = 10.0f;
            camera.azimuth = 45.0f;
            camera.elevation = 30.0f;
            camera.center = Point3D(0, 0, 0);
            camera.updateEye();
            n_segments_draw = max_segments;
            selected_segment = -1;
            break;
        case 27:  // ESC
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_PAGE_UP:
            // Incrementar segmentos visíveis (aumentar quantidade)
            if (max_segments > 0) {
                int increment = std::max(1, max_segments / 20);  // Incremento de ~5% dos segmentos
                n_segments_draw = std::min(max_segments, n_segments_draw + increment);
                std::cout << "Segmentos visíveis: " << n_segments_draw << "/" << max_segments << std::endl;
                // NÃO alterar a câmera ao mudar segmentos
            } else {
                std::cout << "Nenhum segmento disponível" << std::endl;
            }
            break;
        case GLUT_KEY_PAGE_DOWN:
            // Decrementar segmentos visíveis (diminuir quantidade)
            if (max_segments > 0) {
                int decrement = std::max(1, max_segments / 20);  // Decremento de ~5% dos segmentos
                n_segments_draw = std::max(1, n_segments_draw - decrement);
                std::cout << "Segmentos visíveis: " << n_segments_draw << "/" << max_segments << std::endl;
                // NÃO alterar a câmera ao mudar segmentos
            } else {
                std::cout << "Nenhum segmento disponível" << std::endl;
            }
            break;
    }
    
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouse_left_pressed = true;
            last_mouse_x = x;
            last_mouse_y = y;
        } else {
            mouse_left_pressed = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (mouse_left_pressed) {
        int dx = x - last_mouse_x;
        int dy = y - last_mouse_y;
        
        // Rotação baseada no movimento do mouse
        float sensitivity = 0.5f;
        camera.azimuth += dx * sensitivity;
        camera.elevation += dy * sensitivity;
        
        // Limitar elevação
        if (camera.elevation > 89.0f) camera.elevation = 89.0f;
        if (camera.elevation < -89.0f) camera.elevation = -89.0f;
        
        camera.updateEye();
        
        last_mouse_x = x;
        last_mouse_y = y;
        
        glutPostRedisplay();
    }
}
