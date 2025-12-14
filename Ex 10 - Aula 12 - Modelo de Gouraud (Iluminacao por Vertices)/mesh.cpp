#include "mesh.h"
#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

// Criar um cubo simples
Mesh Mesh::createCube() {
    Mesh cube;
    
    // Vértices do cubo
    cube.addVertex(Vector3D(-1, -1, -1)); // 0
    cube.addVertex(Vector3D( 1, -1, -1)); // 1
    cube.addVertex(Vector3D( 1,  1, -1)); // 2
    cube.addVertex(Vector3D(-1,  1, -1)); // 3
    cube.addVertex(Vector3D(-1, -1,  1)); // 4
    cube.addVertex(Vector3D( 1, -1,  1)); // 5
    cube.addVertex(Vector3D( 1,  1,  1)); // 6
    cube.addVertex(Vector3D(-1,  1,  1)); // 7
    
    // Faces do cubo (12 triângulos)
    // Face frontal
    cube.addFace(0, 1, 2);
    cube.addFace(0, 2, 3);
    
    // Face traseira
    cube.addFace(5, 4, 7);
    cube.addFace(5, 7, 6);
    
    // Face esquerda
    cube.addFace(4, 0, 3);
    cube.addFace(4, 3, 7);
    
    // Face direita
    cube.addFace(1, 5, 6);
    cube.addFace(1, 6, 2);
    
    // Face inferior
    cube.addFace(4, 5, 1);
    cube.addFace(4, 1, 0);
    
    // Face superior
    cube.addFace(3, 2, 6);
    cube.addFace(3, 6, 7);
    
    return cube;
}

// Criar um octaedro
Mesh Mesh::createOctahedron() {
    Mesh octahedron;
    
    // Vértices do octaedro
    octahedron.addVertex(Vector3D( 1,  0,  0)); // 0 - direita
    octahedron.addVertex(Vector3D(-1,  0,  0)); // 1 - esquerda
    octahedron.addVertex(Vector3D( 0,  1,  0)); // 2 - cima
    octahedron.addVertex(Vector3D( 0, -1,  0)); // 3 - baixo
    octahedron.addVertex(Vector3D( 0,  0,  1)); // 4 - frente
    octahedron.addVertex(Vector3D( 0,  0, -1)); // 5 - trás
    
    // Faces do octaedro (8 triângulos)
    // Metade superior
    octahedron.addFace(2, 0, 4); // cima-direita-frente
    octahedron.addFace(2, 4, 1); // cima-frente-esquerda
    octahedron.addFace(2, 1, 5); // cima-esquerda-trás
    octahedron.addFace(2, 5, 0); // cima-trás-direita
    
    // Metade inferior
    octahedron.addFace(3, 4, 0); // baixo-frente-direita
    octahedron.addFace(3, 1, 4); // baixo-esquerda-frente
    octahedron.addFace(3, 5, 1); // baixo-trás-esquerda
    octahedron.addFace(3, 0, 5); // baixo-direita-trás
    
    return octahedron;
}

void Mesh::addVertex(const Vector3D& vertex) {
    vertices.push_back(vertex);
}

void Mesh::addFace(int v1, int v2, int v3) {
    faces.push_back(Face(v1, v2, v3));
}

// Passo 2: Calcular normais de face
void Mesh::calculateFaceNormals() {
    for (auto& face : faces) {
        // Obter os três vértices da face
        const Vector3D& v1 = vertices[face.v1];
        const Vector3D& v2 = vertices[face.v2];
        const Vector3D& v3 = vertices[face.v3];
        
        // Calcular dois vetores da face
        Vector3D edge1 = v2 - v1;
        Vector3D edge2 = v3 - v1;
        
        // Calcular a normal usando produto vetorial
        Vector3D normal = edge1.cross(edge2);
        
        // Normalizar o vetor
        face.normal = normal.normalize();
    }
}

// Passo 3 e 4: Calcular normais de vértice
void Mesh::calculateVertexNormals() {
    // Inicializar normais dos vértices com zero
    vertexNormals.clear();
    vertexNormals.resize(vertices.size(), Vector3D(0, 0, 0));
    
    // Somar as normais das faces adjacentes a cada vértice
    for (const auto& face : faces) {
        // Adicionar a normal da face aos três vértices que a compõem
        vertexNormals[face.v1] = vertexNormals[face.v1] + face.normal;
        vertexNormals[face.v2] = vertexNormals[face.v2] + face.normal;
        vertexNormals[face.v3] = vertexNormals[face.v3] + face.normal;
    }
    
    // Normalizar cada normal de vértice
    for (auto& normal : vertexNormals) {
        normal = normal.normalize();
    }
}

// Passo 5: Renderizar usando OpenGL
void Mesh::render(bool useSmooth) const {
    glBegin(GL_TRIANGLES);
    
    for (const auto& face : faces) {
        if (useSmooth) {
            // Usar normais por vértice (Gouraud Shading)
            glNormal3f(vertexNormals[face.v1].x, vertexNormals[face.v1].y, vertexNormals[face.v1].z);
            glVertex3f(vertices[face.v1].x, vertices[face.v1].y, vertices[face.v1].z);
            
            glNormal3f(vertexNormals[face.v2].x, vertexNormals[face.v2].y, vertexNormals[face.v2].z);
            glVertex3f(vertices[face.v2].x, vertices[face.v2].y, vertices[face.v2].z);
            
            glNormal3f(vertexNormals[face.v3].x, vertexNormals[face.v3].y, vertexNormals[face.v3].z);
            glVertex3f(vertices[face.v3].x, vertices[face.v3].y, vertices[face.v3].z);
        } else {
            // Usar normal da face (Flat Shading)
            glNormal3f(face.normal.x, face.normal.y, face.normal.z);
            
            glVertex3f(vertices[face.v1].x, vertices[face.v1].y, vertices[face.v1].z);
            glVertex3f(vertices[face.v2].x, vertices[face.v2].y, vertices[face.v2].z);
            glVertex3f(vertices[face.v3].x, vertices[face.v3].y, vertices[face.v3].z);
        }
    }
    
    glEnd();
}

void Mesh::clear() {
    vertices.clear();
    faces.clear();
    vertexNormals.clear();
}
