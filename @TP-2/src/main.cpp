/*
 * TP2 - Visualização 3D de Árvores Arteriais
 * BCC327 - Computação Gráfica
 * 
 * Funcionalidades:
 * - Leitura de arquivos VTK 3D (Legacy ASCII)
 * - Modelagem de ramos como cilindros 3D
 * - Projeção perspectiva
 * - Câmera orbitante
 * - Modelos de iluminação (Flat e Phong)
 * - Transparência e Z-buffer
 * - Interação 3D e animação de crescimento
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("TP2 - Visualizador 3D Árvores Arteriais");
    
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
            std::cout << "  Use [/] para navegar entre os arquivos de crescimento." << std::endl;
            std::cout << "  Use M para ativar animação automática.\n" << std::endl;
        }
    } else {
        std::cout << "Uso: " << argv[0] << " <arquivo.vtk>" << std::endl;
        std::cout << "Exemplo: " << argv[0] << " Nterm_128/tree3D_Nterm0128_step0128.vtk" << std::endl;
        std::cout << "\nO programa detectará automaticamente arquivos de crescimento na mesma série." << std::endl;
        return 1;
    }
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    
    glutMainLoop();
    
    return 0;
}
