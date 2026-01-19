# Makefile para TP2 - Visualizador 3D Árvores Arteriais
# Compatível com macOS, Linux e Windows (MSYS2)

TARGET = tp2_visualizador
SRC = src/main.cpp src/globals.cpp src/utils.cpp src/interface.cpp src/handlers.cpp
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Detecção do sistema operacional
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    LIBS = -framework OpenGL -framework GLUT
    CXXFLAGS += -Wno-deprecated-declarations
else
    # Windows + MSYS2 (e também Linux se usar freeglut)
    LIBS = -lopengl32 -lglu32 -lfreeglut -lm
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)
	@echo "✓ Compilação concluída: $(TARGET)"

clean:
	rm -f $(TARGET)
	@echo "✓ Arquivos limpos"

rebuild: clean all

run: $(TARGET)
	./$(TARGET) Nterm_128/tree3D_Nterm0128_step0128.vtk

build:
	echo "Compilando..."
	make clean
	make all
	@echo "✓ Compilação concluída: $(TARGET)"

help:
	@echo "Makefile para TP2 - Visualizador 3D"
	@echo ""
	@echo "Comandos:"
	@echo "  make        - Compila o programa"
	@echo "  make run    - Compila e executa com exemplo de arquivo"
	@echo "  make clean  - Remove arquivos compilados"
	@echo "  make rebuild - Limpa e recompila"

.PHONY: all clean rebuild run help
