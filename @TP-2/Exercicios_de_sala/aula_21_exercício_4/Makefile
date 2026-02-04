# Variáveis de Compilação
CC = g++
LDFLAGS = -Llib -lopengl32 -lglu32 -lfreeglut
TARGET = exec_culling.exe

# Regra padrão: Compila o projeto
all: $(TARGET)

$(TARGET): main.cpp cube_renderer.cpp
	$(CC) main.cpp cube_renderer.cpp -o $(TARGET) $(LDFLAGS)

# Regra para executar o programa
run: all
	./$(TARGET)

# Limpeza dos arquivos gerados
clean:
	del $(TARGET)