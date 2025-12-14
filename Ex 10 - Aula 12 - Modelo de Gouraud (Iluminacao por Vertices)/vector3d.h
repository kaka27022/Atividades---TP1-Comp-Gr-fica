#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>

struct Vector3D {
    float x, y, z;
    
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // Produto vetorial
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    // Subtração de vetores
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }
    
    // Adição de vetores
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }
    
    // Produto escalar
    float dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    // Magnitude do vetor
    float magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }
    
    // Normalizar o vetor
    Vector3D normalize() const {
        float mag = magnitude();
        if (mag > 0.0f) {
            return Vector3D(x / mag, y / mag, z / mag);
        }
        return Vector3D(0, 0, 0);
    }
    
    // Operadores de acesso
    float& operator[](int index) {
        return (&x)[index];
    }
    
    const float& operator[](int index) const {
        return (&x)[index];
    }
};

#endif // VECTOR3D_H
