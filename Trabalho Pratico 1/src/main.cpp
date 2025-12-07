/*
 * TP1 - Visualização 2D de Árvores Arteriais
 * BCC327 - Computação Gráfica
 * 
 * Funcionalidades:
 * - Leitura de arquivos VTK (Legacy ASCII)
 * - Transformações geométricas 2D (translação, rotação, escala)
 * - Projeção ortográfica
 * - Visualização incremental do crescimento
 * - Recorte de segmentos (Cohen-Sutherland)
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include "globals.h"
#include "utils.h"
#include "interface.h"
#include "handlers.h"

// ============================================================
// MAIN
// ============================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("TP1 - Visualizador 2D Árvores Arteriais");
    
    init();
    
    // Carregar arquivo VTK se fornecido
    if (argc > 1) {
        std::string initial_file = argv[1];
        
        // Tentar encontrar arquivos de crescimento na mesma série
        growth_mode = findGrowthFiles(initial_file);
        
        // Carregar arquivo inicial
        if (!loadCurrentGrowthFile()) {
            std::cerr << "Erro ao carregar arquivo VTK. Saindo..." << std::endl;
            return 1;
        }
        
        if (growth_mode) {
            std::cout << "\n✓ Modo de crescimento incremental ativado!" << std::endl;
            std::cout << "  Use [/] para navegar entre os arquivos de crescimento.\n" << std::endl;
        }
    } else {
        std::cout << "Uso: " << argv[0] << " <arquivo.vtk>" << std::endl;
        std::cout << "Exemplo: " << argv[0] << " Nterm_064/tree2D_Nterm0064_step0064.vtk" << std::endl;
        std::cout << "\nO programa detectará automaticamente arquivos de crescimento na mesma série." << std::endl;
        return 1;
    }
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutMainLoop();
    
    return 0;
}
