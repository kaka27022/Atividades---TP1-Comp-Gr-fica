/*
 * interface.h
 * Funções de manipulação da interface gráfica - TP2 (3D)
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "globals.h"

// Funções de interface gráfica
void drawCylinder(const Point3D& p0, const Point3D& p1, float radius, int segments = 16, 
                  float base_r = -1.0f, float base_g = -1.0f, float base_b = -1.0f);
void drawTree3D();
void drawSelectedSegment();
void displayText();
void display();
void reshape(int w, int h);
void init();
void updateCamera();
void updateAnimation(int value = 0);

// Funções de iluminação
void setupLightingFlat(const Point3D& normal);
void setupLightingPhong(const Point3D& vertex, const Point3D& normal, const Point3D& eye);
void getColorFromRadius(float normalized_radius, float& r, float& g, float& b);

#endif // INTERFACE_H
