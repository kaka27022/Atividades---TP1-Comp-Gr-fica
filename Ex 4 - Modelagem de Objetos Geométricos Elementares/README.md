Este documento explica como compilar e executar o programa do cubo colorido em OpenGL no Windows, usando:
VSCode
MinGW‑w64 (g++)
FreeGLUT
Extensões de C/C++ do VSCode

Pré‑requisitos:

Antes de compilar, verifique se as seguintes ferramentas estão instaladas:

✅ MinGW‑w64 - g++ --version
✅Baixe o FreeGLUT compilado para Windows.

Compile com : g++ main.cpp -o cubo.exe -lfreeglut -lopengl32 -lglu32
Execute com : ./cubo.exe

![alt text](image.png)

![alt text](image-1.png)

![alt text](image-2.png)