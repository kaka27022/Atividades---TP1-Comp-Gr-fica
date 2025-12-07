/*
 * utils.cpp
 * Implementação das funções auxiliares
 */

#include "utils.h"
#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>

// ============================================================
// FUNÇÕES AUXILIARES
// ============================================================

// Extrai o diretório do arquivo
std::string getDirectory(const std::string& filepath) {
    size_t found = filepath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filepath.substr(0, found);
    }
    return ".";
}

// Extrai o nome do arquivo
std::string getFilename(const std::string& filepath) {
    size_t found = filepath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filepath.substr(found + 1);
    }
    return filepath;
}

// Encontra todos os arquivos de crescimento na mesma série
bool findGrowthFiles(const std::string& initial_file) {
    growth_files.clear();
    
    std::string dir = getDirectory(initial_file);
    std::string filename = getFilename(initial_file);
    
    // Extrair prefixo (ex: "tree2D_Nterm0064_step" de "tree2D_Nterm0064_step0008.vtk")
    size_t step_pos = filename.find("_step");
    if (step_pos == std::string::npos) {
        // Não é um arquivo de série, usar apenas este arquivo
        growth_files.push_back(initial_file);
        return false;
    }
    
    std::string prefix = filename.substr(0, step_pos + 5);  // Inclui "_step"
    std::string extension = ".vtk";
    
    // Listar arquivos no diretório
    DIR* d = opendir(dir.c_str());
    if (!d) {
        growth_files.push_back(initial_file);
        return false;
    }
    
    struct dirent* entry;
    while ((entry = readdir(d)) != nullptr) {
        std::string entry_name = entry->d_name;
        
        // Verificar se começa com o prefixo e termina com .vtk
        if (entry_name.find(prefix) == 0 && entry_name.find(extension) != std::string::npos) {
            std::string full_path = dir + "/" + entry_name;
            growth_files.push_back(full_path);
        }
    }
    closedir(d);
    
    // Ordenar arquivos por número do step
    std::sort(growth_files.begin(), growth_files.end(), [](const std::string& a, const std::string& b) {
        // Extrair número do step
        size_t step_a = a.find("_step");
        size_t step_b = b.find("_step");
        if (step_a == std::string::npos || step_b == std::string::npos) return a < b;
        
        std::string num_a = a.substr(step_a + 5);
        std::string num_b = b.substr(step_b + 5);
        num_a = num_a.substr(0, num_a.find('.'));
        num_b = num_b.substr(0, num_b.find('.'));
        
        try {
            int val_a = std::stoi(num_a);
            int val_b = std::stoi(num_b);
            return val_a < val_b;
        } catch (...) {
            return a < b;
        }
    });
    
    // Encontrar índice do arquivo inicial
    current_growth_index = 0;
    for (size_t i = 0; i < growth_files.size(); i++) {
        if (growth_files[i] == initial_file || getFilename(growth_files[i]) == filename) {
            current_growth_index = i;
            break;
        }
    }
    
    std::cout << "Arquivos de crescimento encontrados: " << growth_files.size() << std::endl;
    for (size_t i = 0; i < growth_files.size(); i++) {
        std::cout << "  [" << i << "] " << getFilename(growth_files[i]);
        if (static_cast<int>(i) == current_growth_index) std::cout << " <-- atual";
        std::cout << std::endl;
    }
    
    return growth_files.size() > 1;
}

// Carrega o arquivo de crescimento atual
bool loadCurrentGrowthFile() {
    if (growth_files.empty() || current_growth_index < 0 || 
        current_growth_index >= (int)growth_files.size()) {
        return false;
    }
    
    return readVTKFile(growth_files[current_growth_index]);
}

// ============================================================
// LEITURA DE ARQUIVOS VTK
// ============================================================

bool readVTKFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo " << filename << std::endl;
        return false;
    }

    points.clear();
    lines.clear();
    radii.clear();

    std::string line;
    int npoints = 0;
    int nlines = 0;
    bool reading_points = false;
    bool reading_lines = false;
    bool reading_radii = false;
    int points_read = 0;
    int lines_read = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        // Ler número de pontos
        if (token == "POINTS") {
            iss >> npoints;
            std::string type;
            iss >> type;
            reading_points = true;
            reading_lines = false;
            reading_radii = false;
            points_read = 0;
            continue;
        }

        // Ler número de linhas
        if (token == "LINES") {
            iss >> nlines;
            int total_indices;
            iss >> total_indices;
            reading_points = false;
            reading_lines = true;
            reading_radii = false;
            lines_read = 0;
            continue;
        }

        // Detectar início dos raios
        if (token == "CELL_DATA" || token == "POINT_DATA") {
            reading_points = false;
            reading_lines = false;
            continue;
        }

        if (token == "scalars" && line.find("raio") != std::string::npos) {
            reading_radii = true;
            reading_points = false;
            reading_lines = false;
            // Pular linha "LOOKUP_TABLE default"
            std::getline(file, line);
            continue;
        }

        // Ler pontos
        if (reading_points && points_read < npoints) {
            float x, y, z;
            std::istringstream iss_line(line);
            if (iss_line >> x >> y >> z) {
                points.push_back(Point2D(x, y));
                points_read++;
            }
        }

        // Ler linhas
        if (reading_lines && lines_read < nlines) {
            std::istringstream iss_line(line);
            int k;
            if (iss_line >> k) {
                if (k >= 2) {
                    std::vector<int> indices(k);
                    for (int i = 0; i < k; i++) {
                        iss_line >> indices[i];
                    }
                    // Criar segmentos entre pontos consecutivos
                    for (int i = 0; i < k - 1; i++) {
                        lines.push_back(Line(indices[i], indices[i+1]));
                    }
                }
                lines_read++;
            }
        }

        // Ler raios
        if (reading_radii) {
            float r;
            std::istringstream iss_line(line);
            if (iss_line >> r) {
                radii.push_back(r);
            }
        }
    }

    file.close();

    // Se não há raios, criar valores padrão
    if (radii.empty()) {
        radii.resize(lines.size(), 0.5f);
    }

    // Ajustar raios para corresponder ao número de linhas
    if (radii.size() < lines.size()) {
        while (radii.size() < lines.size()) {
            radii.push_back(0.5f);
        }
    }

    // Atualizar raios nas linhas
    for (size_t i = 0; i < lines.size() && i < radii.size(); i++) {
        lines[i].radius = radii[i];
    }

    max_segments = lines.size();
    n_segments_draw = max_segments;

    std::cout << "Arquivo VTK carregado: " << filename << std::endl;
    std::cout << "  Pontos: " << points.size() << std::endl;
    std::cout << "  Segmentos: " << lines.size() << std::endl;
    std::cout << "  Raios: " << radii.size() << std::endl;

    return true;
}

