# Makefile para Braço Robótico - Computação Gráfica
# Compatível com macOS e Linux

# Nome do executável
TARGET = braco_robotico

# Arquivo fonte
SRC = main.cpp

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Detecção do sistema operacional
UNAME_S := $(shell uname -s)

# Configuração específica por sistema
ifeq ($(UNAME_S),Darwin)
    # macOS - usa frameworks
    LIBS = -framework OpenGL -framework GLUT
    CXXFLAGS += -Wno-deprecated-declarations
else
    # Linux - usa bibliotecas
    LIBS = -lGL -lGLU -lglut -lm
endif

# Regra padrão: compilar o programa
all: $(TARGET)

# Compilar o executável
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)
	@echo "✓ Compilação concluída: $(TARGET)"

# Limpar arquivos gerados
clean:
	rm -f $(TARGET)
	@echo "✓ Arquivos limpos"

# Recompilar (limpar e compilar novamente)
rebuild: clean all

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Ajuda
help:
	@echo "Makefile para Braço Robótico"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make        - Compila o programa"
	@echo "  make run    - Compila e executa o programa"
	@echo "  make clean  - Remove arquivos compilados"
	@echo "  make rebuild - Limpa e recompila"
	@echo "  make help   - Mostra esta ajuda"
	@echo ""

.PHONY: all clean rebuild run help
