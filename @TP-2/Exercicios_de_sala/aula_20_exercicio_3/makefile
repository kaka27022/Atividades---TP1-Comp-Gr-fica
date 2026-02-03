# Nome do executável
TARGET = weiler_atherton

# Compilador
CXX = g++

# Flags do compilador (C++11 para usar std::list e inicializadores)
CXXFLAGS = -Wall -std=c++14

# Bibliotecas necessárias (OpenGL, GLU, GLUT)
LIBS = -lGL -lGLU -lglut

# Arquivo fonte
SRC = main.cpp

# Regra principal: compila o projeto
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Regra para rodar o programa
run: all
	./$(TARGET)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET)

.PHONY: all run clean