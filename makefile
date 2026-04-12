CXX      = g++
CXXFLAGS = -std=c++17 -Wall -DGL_SILENCE_DEPRECATION
SRC      = main.cpp
LIBS    = -framework OpenGL -framework GLUT
TARGET  = main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)