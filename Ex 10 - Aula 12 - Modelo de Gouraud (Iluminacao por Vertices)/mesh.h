#ifndef MESH_H
#define MESH_H

#include <vector>
#include "vector3d.h"

struct Face {
    int v1, v2, v3;  // Índices dos vértices
    Vector3D normal; // Normal da face
    
    Face(int v1, int v2, int v3) : v1(v1), v2(v2), v3(v3) {}
};

class Mesh {
private:
    std::vector<Vector3D> vertices;
    std::vector<Face> faces;
    std::vector<Vector3D> vertexNormals;
    
public:
    // Construtores para diferentes formas
    static Mesh createCube();
    static Mesh createOctahedron();
    
    // Métodos principais
    void calculateFaceNormals();
    void calculateVertexNormals();
    void render(bool useSmooth = true) const;
    
    // Getters
    const std::vector<Vector3D>& getVertices() const { return vertices; }
    const std::vector<Face>& getFaces() const { return faces; }
    const std::vector<Vector3D>& getVertexNormals() const { return vertexNormals; }
    
    // Adicionar vértices e faces manualmente
    void addVertex(const Vector3D& vertex);
    void addFace(int v1, int v2, int v3);
    
    void clear();
};

#endif // MESH_H
