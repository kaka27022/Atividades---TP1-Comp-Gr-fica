# Makefile for OpenGL Shading Comparison Project
# Computação Gráfica - UFOP

# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++11

# OpenGL libraries for macOS
LIBS = -framework OpenGL -framework GLUT

# Alternative for Linux (uncomment if needed):
# LIBS = -lGL -lGLU -lglut

# Source files
SOURCES = main.cpp

# Output executable
TARGET = shading_comparison

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)
	@echo "Build completed successfully!"
	@echo "Run with: ./$(TARGET)"

# Clean target
clean:
	rm -f $(TARGET)
	@echo "Clean completed!"

# Run target
run: $(TARGET)
	./$(TARGET)

# Install dependencies (for Linux systems)
install-deps:
	@echo "For Ubuntu/Debian systems, install dependencies with:"
	@echo "sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev"
	@echo ""
	@echo "For macOS, make sure Xcode Command Line Tools are installed:"
	@echo "xcode-select --install"

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove built files"
	@echo "  run        - Build and run the project"
	@echo "  install-deps - Show how to install dependencies"
	@echo "  help       - Show this help message"

.PHONY: all clean run install-deps help
