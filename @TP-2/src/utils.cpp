/*
 * utils.cpp
 * Implementação das funções auxiliares - TP2 (3D)
 */

#include "globals.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
// FUNÇÕES AUXILIARES
// ============================================================

std::string getDirectory(const std::string& filepath) {
    size_t found = filepath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filepath.substr(0, found);
    }
    return ".";
}

std::string getFilename(const std::string& filepath) {
    size_t found = filepath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filepath.substr(found + 1);
    }
    return filepath;
}

bool findGrowthFiles(const std::string& initial_file) {
    growth_files.clear();
    
    std::string dir = getDirectory(initial_file);
    std::string filename = getFilename(initial_file);
    
    // Extrair prefixo (ex: "tree3D_Nterm0128_step" de "tree3D_Nterm0128_step0016.vtk")
    size_t step_pos = filename.find("_step");
    if (step_pos == std::string::npos) {
        growth_files.push_back(initial_file);
        return false;
    }
    
    std::string prefix = filename.substr(0, step_pos + 5);
    std::string extension = ".vtk";
    
    DIR* d = opendir(dir.c_str());
    if (!d) {
        growth_files.push_back(initial_file);
        return false;
    }
    
    struct dirent* entry;
    while ((entry = readdir(d)) != nullptr) {
        std::string entry_name = entry->d_name;
        
        if (entry_name.find(prefix) == 0 && entry_name.find(extension) != std::string::npos) {
            std::string full_path = dir + "/" + entry_name;
            growth_files.push_back(full_path);
        }
    }
    closedir(d);
    
    // Ordenar arquivos por número do step
    std::sort(growth_files.begin(), growth_files.end(), [](const std::string& a, const std::string& b) {
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

bool loadCurrentGrowthFile() {
    if (growth_files.empty() || current_growth_index < 0 || 
        current_growth_index >= (int)growth_files.size()) {
        return false;
    }
    
    // Preservar a distância e ângulos da câmera ao carregar novo arquivo
    // O centro será atualizado automaticamente no readVTKFile3D
    float saved_distance = camera.distance;
    float saved_azimuth = camera.azimuth;
    float saved_elevation = camera.elevation;
    
    // Carregar arquivo sem resetar distância e ângulos da câmera
    bool result = readVTKFile3D(growth_files[current_growth_index], false);
    
    if (result) {
        // Restaurar distância e ângulos, mantendo o novo centro
        camera.distance = saved_distance;
        camera.azimuth = saved_azimuth;
        camera.elevation = saved_elevation;
        camera.updateEye();
        
        std::cout << "Câmera preservada - distância: " << camera.distance << std::endl;
    }
    
    return result;
}

// ============================================================
// LEITURA DE ARQUIVOS VTK 3D
// ============================================================

bool readVTKFile3D(const std::string& filename, bool update_camera) {
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

        // Ler pontos 3D
        if (reading_points && points_read < npoints) {
            float x, y, z;
            std::istringstream iss_line(line);
            if (iss_line >> x >> y >> z) {
                points.push_back(Point3D(x, y, z));
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
                        lines.push_back(Line3D(indices[i], indices[i+1]));
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

    // Calcular bounding box e ajustar câmera automaticamente
    if (!points.empty()) {
        float min_x = points[0].x, max_x = points[0].x;
        float min_y = points[0].y, max_y = points[0].y;
        float min_z = points[0].z, max_z = points[0].z;
        
        for (const auto& p : points) {
            if (p.x < min_x) min_x = p.x;
            if (p.x > max_x) max_x = p.x;
            if (p.y < min_y) min_y = p.y;
            if (p.y > max_y) max_y = p.y;
            if (p.z < min_z) min_z = p.z;
            if (p.z > max_z) max_z = p.z;
        }
        
        // Calcular centro
        float center_x = (min_x + max_x) / 2.0f;
        float center_y = (min_y + max_y) / 2.0f;
        float center_z = (min_z + max_z) / 2.0f;
        
        // Calcular tamanho máximo
        float size_x = max_x - min_x;
        float size_y = max_y - min_y;
        float size_z = max_z - min_z;
        float max_size = std::max(size_x, std::max(size_y, size_z));
        
        // Armazenar escala globalmente
        data_scale = max_size;
        if (data_scale < 0.001f) data_scale = 1.0f;
        
        // Ajustar câmera apenas se solicitado (na primeira carga do arquivo)
        if (update_camera) {
            // Ajustar câmera para focar no centro
            camera.center = Point3D(center_x, center_y, center_z);
            camera.distance = max_size * 1.8f;  // Distância proporcional ao tamanho (aumentada para melhor visão geral)
            camera.azimuth = 45.0f;  // Reset ângulo horizontal
            camera.elevation = 30.0f;  // Reset ângulo vertical
            camera.updateEye();
            
            std::cout << "Bounding box: [" << min_x << ", " << max_x << "] x ["
                      << min_y << ", " << max_y << "] x [" << min_z << ", " << max_z << "]" << std::endl;
            std::cout << "Centro: (" << center_x << ", " << center_y << ", " << center_z << ")" << std::endl;
            std::cout << "Tamanho máximo: " << max_size << std::endl;
            std::cout << "Distância da câmera: " << camera.distance << std::endl;
        } else {
            // Apenas atualizar o centro, mantendo distância e ângulos
            // NÃO chamar updateEye() aqui - será chamado pela função que chamou readVTKFile3D
            camera.center = Point3D(center_x, center_y, center_z);
            
            std::cout << "Arquivo carregado (câmera será preservada)" << std::endl;
        }
    }

    std::cout << "Arquivo VTK 3D carregado: " << filename << std::endl;
    std::cout << "  Pontos: " << points.size() << std::endl;
    std::cout << "  Segmentos: " << lines.size() << std::endl;
    std::cout << "  Raios: " << radii.size() << std::endl;

    return true;
}

// ============================================================
// FUNÇÕES DE CÁLCULO VETORIAL 3D
// ============================================================

float dotProduct(const Point3D& a, const Point3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point3D crossProduct(const Point3D& a, const Point3D& b) {
    return Point3D(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

Point3D calculateNormal(const Point3D& p0, const Point3D& p1, const Point3D& p2) {
    Point3D v1 = p1 - p0;
    Point3D v2 = p2 - p0;
    Point3D normal = crossProduct(v1, v2);
    normal.normalize();
    return normal;
}
