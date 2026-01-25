CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -Llib -lopengl32 -lglu32 -lfreeglut
TARGET = programa_cyrus_beck.exe
SRCS = main.cpp algs.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# --- NOVA REGRA AQUI ---
run: all
	./$(TARGET)
# -----------------------

%.o: %.cpp algs.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)