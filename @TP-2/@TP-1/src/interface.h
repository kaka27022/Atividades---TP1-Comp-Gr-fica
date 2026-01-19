/*
 * interface.h
 * Funções de manipulação da interface gráfica
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "globals.h"

// Enum para recorte Cohen-Sutherland
enum OutCode {
    INSIDE = 0,
    LEFT = 1,
    RIGHT = 2,
    BOTTOM = 4,
    TOP = 8
};

// Funções de interface gráfica
OutCode computeOutCode(float x, float y);
bool cohenSutherlandClip(float& x0, float& y0, float& x1, float& y1);
void drawTree();
void displayText();
void display();
void reshape(int w, int h);
void init();
void applyTransform(Point2D& p);

#endif // INTERFACE_H

