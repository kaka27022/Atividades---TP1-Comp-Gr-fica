/*
 * handlers.h
 * Handlers de eventos (teclado e mouse) - TP2 (3D)
 */

#ifndef HANDLERS_H
#define HANDLERS_H

// Handlers de eventos
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);

#endif // HANDLERS_H
