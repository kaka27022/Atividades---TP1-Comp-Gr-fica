/*
 * utils.h
 * Funções auxiliares para manipulação de arquivos e diretórios
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>

// Funções auxiliares
std::string getDirectory(const std::string& filepath);
std::string getFilename(const std::string& filepath);
bool readVTKFile(const std::string& filename);
bool findGrowthFiles(const std::string& initial_file);
bool loadCurrentGrowthFile();

#endif // UTILS_H

