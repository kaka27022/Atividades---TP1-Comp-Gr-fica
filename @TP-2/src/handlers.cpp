/*
 * handlers.cpp
 * Implementação dos handlers de eventos
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "handlers.h"
#include "globals.h"
#include "utils.h"
#include <algorithm>
#include <cstdlib>

// ============================================================
// CONTROLES DE TECLADO
// ============================================================

void keyboard(unsigned char key, int, int) {
    float rot_step = 5.0f;
    
    switch (key) {
        case 'r':
            angle += rot_step;
            break;
        case 'R':
            angle -= rot_step;
            break;
        case '+':
        case '=':
            scale *= 1.1f;
            break;
        case '-':
        case '_':
            scale /= 1.1f;
            break;
        case 'c':
        case 'C':
            clip_enabled = !clip_enabled;
            break;
        case 'x':
        case 'X':
            // Aumentar tamanho da janela de recorte
            clip_size = std::min(1.0f, clip_size + 0.1f);
            break;
        case 'z':
        case 'Z':
            // Diminuir tamanho da janela de recorte
            clip_size = std::max(0.1f, clip_size - 0.1f);
            break;
        case '[':
            // Arquivo anterior de crescimento
            if (growth_mode && growth_files.size() > 1) {
                current_growth_index--;
                if (current_growth_index < 0) {
                    current_growth_index = growth_files.size() - 1;
                }
                loadCurrentGrowthFile();
            }
            break;
        case ']':
            // Próximo arquivo de crescimento
            if (growth_mode && growth_files.size() > 1) {
                current_growth_index++;
                if (current_growth_index >= (int)growth_files.size()) {
                    current_growth_index = 0;
                }
                loadCurrentGrowthFile();
            }
            break;
        case ' ':
            // Reset
            tx = ty = 0.0f;
            angle = 0.0f;
            scale = 1.0f;
            n_segments_draw = max_segments;
            break;
        case 27:  // ESC
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    float step = 0.1f;
    
    switch (key) {
        case GLUT_KEY_LEFT:
            tx -= step;
            break;
        case GLUT_KEY_RIGHT:
            tx += step;
            break;
        case GLUT_KEY_UP:
            ty += step;
            break;
        case GLUT_KEY_DOWN:
            ty -= step;
            break;
        case GLUT_KEY_PAGE_UP:
            n_segments_draw = std::min(max_segments, n_segments_draw + 1);
            break;
        case GLUT_KEY_PAGE_DOWN:
            n_segments_draw = std::max(1, n_segments_draw - 1);
            break;
    }
    
    glutPostRedisplay();
}

