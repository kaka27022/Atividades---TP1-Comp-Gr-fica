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
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// ============================================================
// ESTRUTURAS DE DADOS
// ============================================================
struct Point2D {
    float x, y;
    Point2D() : x(0), y(0) {}
    Point2D(float x, float y) : x(x), y(y) {}
};

struct Line {
    int p0, p1;  // índices dos pontos
    float radius;
    Line(int p0, int p1, float r = 0.5f) : p0(p0), p1(p1), radius(r) {}
};

// ============================================================
// VARIÁVEIS GLOBAIS
// ============================================================

std::vector<Point2D> points;      // Pontos da árvore
std::vector<Line> lines;           // Segmentos da árvore
std::vector<float> radii;          // Raios dos segmentos

// Parâmetros de transformação
float tx = 0.0f, ty = 0.0f;        // Translação
float angle = 0.0f;                // Rotação (em graus)
float scale = 12.0f;                // Escala

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

// ============================================================
// DECLARAÇÕES FORWARD
// ============================================================

bool readVTKFile(const std::string& filename);

// ============================================================
// DETECÇÃO DE ARQUIVOS DE CRESCIMENTO
// ============================================================

// Extrai o diretório e prefixo do arquivo
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
        if (i == current_growth_index) std::cout << " <-- atual";
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
    int radii_read = 0;

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
            radii_read = 0;
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
                radii_read++;
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

// ============================================================
// TRANSFORMAÇÕES GEOMÉTRICAS (Matrizes Homogêneas 3x3)
// ============================================================

void applyTransform(Point2D& p) {
    // Matriz de transformação: T * R * S
    // Ordem: Escala -> Rotação -> Translação
    
    float rad = angle * M_PI / 180.0f;
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    
    // Aplicar escala
    float sx = p.x * scale;
    float sy = p.y * scale;
    
    // Aplicar rotação
    float rx = sx * cos_a - sy * sin_a;
    float ry = sx * sin_a + sy * cos_a;
    
    // Aplicar translação
    p.x = rx + tx;
    p.y = ry + ty;
}

// ============================================================
// RECORTE COHEN-SUTHERLAND
// ============================================================

enum OutCode {
    INSIDE = 0,
    LEFT = 1,
    RIGHT = 2,
    BOTTOM = 4,
    TOP = 8
};

OutCode computeOutCode(float x, float y) {
    OutCode code = INSIDE;
    if (x < clip_xmin) code = static_cast<OutCode>(code | LEFT);
    if (x > clip_xmax) code = static_cast<OutCode>(code | RIGHT);
    if (y < clip_ymin) code = static_cast<OutCode>(code | BOTTOM);
    if (y > clip_ymax) code = static_cast<OutCode>(code | TOP);
    return code;
}

bool cohenSutherlandClip(float& x0, float& y0, float& x1, float& y1) {
    OutCode outcode0 = computeOutCode(x0, y0);
    OutCode outcode1 = computeOutCode(x1, y1);
    
    bool accept = false;
    
    while (true) {
        if (!(outcode0 | outcode1)) {
            // Ambos os pontos estão dentro
            accept = true;
            break;
        } else if (outcode0 & outcode1) {
            // Ambos os pontos estão fora do mesmo lado
            break;
        } else {
            // Pelo menos um ponto está fora, calcular interseção
            float x, y;
            OutCode outcodeOut = outcode0 ? outcode0 : outcode1;
            
            // Encontrar ponto de interseção
            // Proteção contra divisão por zero
            if (outcodeOut & TOP) {
                float dy = y1 - y0;
                if (fabs(dy) > 1e-6) {
                    x = x0 + (x1 - x0) * (clip_ymax - y0) / dy;
                    y = clip_ymax;
                } else {
                    // Linha horizontal, usar coordenada x do ponto
                    x = x0;
                    y = clip_ymax;
                }
            } else if (outcodeOut & BOTTOM) {
                float dy = y1 - y0;
                if (fabs(dy) > 1e-6) {
                    x = x0 + (x1 - x0) * (clip_ymin - y0) / dy;
                    y = clip_ymin;
                } else {
                    x = x0;
                    y = clip_ymin;
                }
            } else if (outcodeOut & RIGHT) {
                float dx = x1 - x0;
                if (fabs(dx) > 1e-6) {
                    y = y0 + (y1 - y0) * (clip_xmax - x0) / dx;
                    x = clip_xmax;
                } else {
                    // Linha vertical, usar coordenada y do ponto
                    x = clip_xmax;
                    y = y0;
                }
            } else if (outcodeOut & LEFT) {
                float dx = x1 - x0;
                if (fabs(dx) > 1e-6) {
                    y = y0 + (y1 - y0) * (clip_xmin - x0) / dx;
                    x = clip_xmin;
                } else {
                    x = clip_xmin;
                    y = y0;
                }
            }
            
            // Substituir ponto fora pelo ponto de interseção
            if (outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = computeOutCode(x0, y0);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = computeOutCode(x1, y1);
            }
        }
    }
    
    return accept;
}

// ============================================================
// DESENHO
// ============================================================

void drawTree() {
    if (points.empty() || lines.empty()) return;

    // ----- Cálculo dos limites para recorte -----
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble x0, y0, z0, x1, y1, z1;
    GLdouble center_x, center_y, center_z;

    // Unproject canto inferior
    gluUnProject(0, 0, 0, modelview, projection, viewport, &x0, &y0, &z0);
    // Unproject canto superior
    gluUnProject(window_width, window_height, 0, modelview, projection, viewport, &x1, &y1, &z1);
    // Centro
    gluUnProject(window_width/2.0, window_height/2.0, 0, modelview, projection, viewport, &center_x, &center_y, &center_z);

    // Dimensões em coordenadas do mundo
    float viewport_width  = std::abs(x1 - x0);
    float viewport_height = std::abs(y1 - y0);

    // Janela de recorte
    float clip_width  = viewport_width  * clip_size;
    float clip_height = viewport_height * clip_size;

    clip_xmin = center_x - clip_width  / 2.0f;
    clip_xmax = center_x + clip_width  / 2.0f;
    clip_ymin = center_y - clip_height / 2.0f;
    clip_ymax = center_y + clip_height / 2.0f;

    // ---------------- DESENHO DA ÁRVORE ------------------

    int count = 0;

    for (size_t i = 0; i < lines.size() && count < n_segments_draw; i++) {

        Point2D p0 = points[lines[i].p0];
        Point2D p1 = points[lines[i].p1];

        applyTransform(p0);
        applyTransform(p1);

        float x0f = p0.x, y0f = p0.y;
        float x1f = p1.x, y1f = p1.y;

        bool draw = true;
        if (clip_enabled) {
            draw = cohenSutherlandClip(x0f, y0f, x1f, y1f);
        }

        if (draw) {

            // ---------- ESPESSURA FINAL DO TRAÇO ----------
            float base_thickness = 1.0f + lines[i].radius * 8.0f;

            // 🔥 Este fator é controlado pelo teclado (T e G)
            float final_thickness = base_thickness * line_thickness_factor;

            glLineWidth(final_thickness);

            // ---------- COR (degradê pelo radius) ----------
            float r = lines[i].radius;  
            float t = std::min(1.0f, r * 4.0f);

            float R = 1.0f * (1.0 - t) + 0.2f * t;
            float G = 0.2f * (1.0 - t) + 0.6f * t;
            float B = 0.2f * (1.0 - t) + 1.0f * t;

            glColor3f(R, G, B);

            glBegin(GL_LINES);
            glVertex2f(x0f, y0f);
            glVertex2f(x1f, y1f);
            glEnd();
        }

        count++;
    }

    // ---- Desenhar janela de recorte ----
    if (clip_enabled) {
        glLineWidth(1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glLineStipple(1, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);

        glBegin(GL_LINE_LOOP);
        glVertex2f(clip_xmin, clip_ymin);
        glVertex2f(clip_xmax, clip_ymin);
        glVertex2f(clip_xmax, clip_ymax);
        glVertex2f(clip_xmin, clip_ymax);
        glEnd();

        glDisable(GL_LINE_STIPPLE);
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
    
    std::string status = "tx=" + std::to_string(tx).substr(0, 4) + 
                        " ty=" + std::to_string(ty).substr(0, 4) +
                        " rot=" + std::to_string(angle).substr(0, 5) + "°" +
                        " scale=" + std::to_string(scale).substr(0, 4) +
                        " segs=" + std::to_string(n_segments_draw) + "/" + std::to_string(max_segments) +
                        " clip=" + (clip_enabled ? "ON" : "OFF");
    
    if (clip_enabled) {
        status += " size=" + std::to_string(clip_size * 100).substr(0, 3) + "%";
    }
    
    // Adicionar informação de crescimento incremental
    if (growth_mode && growth_files.size() > 1) {
        status += " | Crescimento: " + std::to_string(current_growth_index + 1) + 
                  "/" + std::to_string(growth_files.size());
        if (!growth_files.empty()) {
            std::string current_file = getFilename(growth_files[current_growth_index]);
            status += " [" + current_file + "]";
        }
    }
    
    glRasterPos2f(10, window_height - 20);
    for (char c : status) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    
    std::string controls = "Controles: Setas(transl) r/R(rot) +/- (scale) c(clip) x/z(clip size) [](growth) PageUp/Down(segments)";
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
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drawTree();
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
    
    // Projeção ortográfica 2D
    // Ajustar para manter aspect ratio
    float aspect = (float)w / (float)h;
    float view_size = 1.0f;
    
    if (aspect > 1.0f) {
        gluOrtho2D(-view_size * aspect, view_size * aspect, -view_size, view_size);
    } else {
        gluOrtho2D(-view_size, view_size, -view_size / aspect, view_size / aspect);
    }
    
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
// CONTROLES DE TECLADO
// ============================================================

void keyboard(unsigned char key, int, int) {
    float rot_step = 5.0f;

    switch (key) {

        // --- Rotação ---
        case 'r':  // horário
            angle -= rot_step;
            break;

        case 'R':  // anti-horário
            angle += rot_step;
            break;

        // --- Zoom ---
        case '+':
        case '=':
            scale *= 1.1f;
            break;

        case '-':
        case '_':
            scale /= 1.1f;
            break;

        // --- Ativar/desativar recorte ---
        case 'c':
        case 'C':
            clip_enabled = !clip_enabled;
            break;

        // --- Ajustar tamanho da janela de recorte ---
        case 'x':
        case 'X':
            clip_size = std::min(1.0f, clip_size + 0.1f);
            break;

        case 'z':
        case 'Z':
            clip_size = std::max(0.1f, clip_size - 0.1f);
            break;

        // --- Troca de arquivos de crescimento ---
        case '[':
            if (growth_mode && growth_files.size() > 1) {
                current_growth_index--;
                if (current_growth_index < 0)
                    current_growth_index = growth_files.size() - 1;
                loadCurrentGrowthFile();
            }
            break;

        case ']':
            if (growth_mode && growth_files.size() > 1) {
                current_growth_index++;
                if (current_growth_index >= (int)growth_files.size())
                    current_growth_index = 0;
                loadCurrentGrowthFile();
            }
            break;

        // --- Reset total ---
        case ' ':
            tx = ty = 0.0f;
            angle = 0.0f;
            scale = 1.0f;
            n_segments_draw = max_segments;

            // reset da espessura
            thickness_level = 2; // média
            break;

        // ===========================================
        // >>> ESPESSURA DO TRAÇO — apenas T e G <<<
        // ===========================================

        case 't':   // Aumenta espessura
            line_thickness_factor *= 1.1f;
            break;

        case 'g':   // Diminui espessura
            line_thickness_factor /= 1.1f;
            if (line_thickness_factor < 0.1f)
                line_thickness_factor = 0.1f;
            break;

        // --- Sair ---
        case 27: // ESC
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

// ============================================================
// INICIALIZAÇÃO
// ============================================================
 
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Fundo preto
    glColor3f(1.0f, 1.0f, 1.0f);
    
    std::cout << "\n";
    std::cout << "===========================================\n";
    std::cout << "   TP1 - Visualizador 2D Árvores Arteriais\n";
    std::cout << "===========================================\n";
    std::cout << "\n";
    std::cout << "CONTROLES:\n";
    std::cout << "  Setas     - Translação (← → ↑ ↓)\n";
    std::cout << "  r/R       - Rotação (horário/anti-horário)\n";
    std::cout << "  +/-       - Escala (aumentar/diminuir)\n";
    std::cout << "  c         - Toggle recorte (clipping)\n";
    std::cout << "  x/z       - Aumentar/diminuir tamanho da janela de recorte\n";
    std::cout << "  [/]       - Arquivo anterior/próximo de crescimento\n";
    std::cout << "  PageUp/Dn - Segmentos incrementais (dentro do arquivo)\n";
    std::cout << "  ESPAÇO    - Reset transformações\n";
    std::cout << "  ESC       - Sair\n";
    std::cout << "\n";
    std::cout << "===========================================\n";
    std::cout << "\n";
}

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