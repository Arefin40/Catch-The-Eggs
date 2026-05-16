CXX      = g++
CXXFLAGS = -std=c++17 -Wall -DGL_SILENCE_DEPRECATION -Iheaders
TARGET   = main

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CXX      = clang++
    CXXFLAGS += -fblocks
    LIBS     = -framework OpenGL -framework GLUT -framework CoreAudio -framework AudioToolbox -framework CoreFoundation
else ifeq ($(OS),Windows_NT)
    TARGET   = main.exe
    LIBS     = -lfreeglut -lopengl32 -lglu32
else
    LIBS     = -lGL -lglut -lm
endif

.PHONY: run clean

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
