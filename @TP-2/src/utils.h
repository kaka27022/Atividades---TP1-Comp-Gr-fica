/*
 * utils.h
 * Funções auxiliares para manipulação de arquivos e diretórios - TP2 (3D)
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>

// Forward declaration
struct Point3D;

// Funções auxiliares
std::string getDirectory(const std::string& filepath);
std::string getFilename(const std::string& filepath);
bool readVTKFile3D(const std::string& filename, bool update_camera = true);
bool findGrowthFiles(const std::string& initial_file);
bool loadCurrentGrowthFile();

// Funções de cálculo vetorial 3D
float dotProduct(const Point3D& a, const Point3D& b);
Point3D crossProduct(const Point3D& a, const Point3D& b);
Point3D calculateNormal(const Point3D& p0, const Point3D& p1, const Point3D& p2);

#endif // UTILS_H
