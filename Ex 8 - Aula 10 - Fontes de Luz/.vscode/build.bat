@echo off
mkdir build 2>nul
g++ src/main.cpp -o build/lanterna.exe -lfreeglut -lopengl32 -lglu32
echo Compilação concluída.
pause