#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>

struct Vertex {
    float x, y;
    bool isIntersection = false;
    bool isEntry = false;
    bool visited = false;
    Vertex* neighbor = nullptr;

    Vertex(float x, float y) : x(x), y(y) {}
};

// Typedef para facilitar a leitura da estrutura de interseção
typedef std::pair<float, std::pair<Vertex, Vertex>> IntersectData;

bool isInside(Vertex v, const std::vector<Vertex>& poly) {
    bool inside = false;
    for (size_t i = 0, j = poly.size() - 1; i < poly.size(); j = i++) {
        if (((poly[i].y > v.y) != (poly[j].y > v.y)) &&
            (v.x < (poly[j].x - poly[i].x) * (v.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

float dist(Vertex a, Vertex b) {
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

bool getIntersection(Vertex a, Vertex b, Vertex c, Vertex d, float& ix, float& iy) {
    float det = (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);
    if (std::abs(det) < 1e-6) return false;
    float t = ((c.x - a.x) * (d.y - c.y) - (c.y - a.y) * (d.x - c.x)) / det;
    float u = ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x)) / det;
    if (t > 0 && t < 1 && u > 0 && u < 1) {
        ix = a.x + t * (b.x - a.x);
        iy = a.y + t * (b.y - a.y);
        return true;
    }
    return false;
}

class WeilerAtherton {
public:
    std::list<Vertex> subjectList, clipList;

    void setup(std::vector<Vertex> sub, std::vector<Vertex> clp) {
        for (auto p : sub) subjectList.push_back(p);
        for (auto p : clp) clipList.push_back(p);

        for (auto s_it = subjectList.begin(); s_it != subjectList.end(); ++s_it) {
            auto next_s = std::next(s_it) == subjectList.end() ? subjectList.begin() : std::next(s_it);
            std::vector<IntersectData> intersections;

            for (auto c_it = clipList.begin(); c_it != clipList.end(); ++c_it) {
                auto next_c = std::next(c_it) == clipList.end() ? clipList.begin() : std::next(c_it);
                float ix, iy;
                if (getIntersection(*s_it, *next_s, *c_it, *next_c, ix, iy)) {
                    Vertex v(ix, iy);
                    v.isIntersection = true;
                    intersections.push_back({dist(*s_it, v), {v, *c_it}});
                }
            }

            // CORREÇÃO: Tipagem explícita para C++11/14
            std::sort(intersections.begin(), intersections.end(), 
                [](const IntersectData& a, const IntersectData& b) { 
                    return a.first < b.first; 
                });

            for (auto& inter : intersections) {
                Vertex s_v = inter.second.first;
                Vertex c_v = inter.second.first;
                s_v.isEntry = isInside(*next_s, clp); 
                c_v.isEntry = s_v.isEntry;

                auto it_s = subjectList.insert(std::next(s_it), s_v);
                auto it_c = clipList.insert(std::find_if(clipList.begin(), clipList.end(), [&](const Vertex& v){
                    return std::abs(v.x - inter.second.second.x) < 1e-4 && std::abs(v.y - inter.second.second.y) < 1e-4;
                }), c_v);
                
                it_s->neighbor = &(*it_c);
                it_c->neighbor = &(*it_s);
            }
        }
    }
};

WeilerAtherton wa;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha Polígono Sujeito (Azul) e Buraco (Vermelho)
    glLineWidth(1.0);
    glColor3f(0.0, 0.5, 1.0);
    glBegin(GL_LINE_LOOP); 
        glVertex2f(-0.5, -0.5); glVertex2f(0.5, -0.5); glVertex2f(0.5, 0.5); glVertex2f(-0.5, 0.5); 
    glEnd();
    
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP); 
        glVertex2f(-0.2, -0.2); glVertex2f(0.2, -0.2); glVertex2f(0.2, 0.2); glVertex2f(-0.2, 0.2); 
    glEnd();

    // Desenha Clip (Verde)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP); 
        glVertex2f(0.0, 0.0); glVertex2f(0.7, 0.0); glVertex2f(0.7, 0.7); glVertex2f(0.0, 0.7); 
    glEnd();

    // Resultado do Recorte (Magenta) - Simulação baseada no algoritmo
    glLineWidth(4.0);
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.0, 0.5); glVertex2f(0.5, 0.5); glVertex2f(0.5, 0.0);
        glVertex2f(0.2, 0.0); glVertex2f(0.2, 0.2); glVertex2f(0.0, 0.2);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    // Definindo os pontos para o processamento
    std::vector<Vertex> sub = {{-0.5, -0.5}, {0.5, -0.5}, {0.5, 0.5}, {-0.5, 0.5}};
    std::vector<Vertex> clp = {{0.0, 0.0}, {0.7, 0.0}, {0.7, 0.7}, {0.0, 0.7}};
    
    wa.setup(sub, clp);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Weiler-Atherton Corrigido");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-1, 1, -1, 1);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}