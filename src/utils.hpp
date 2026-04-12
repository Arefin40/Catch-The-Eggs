#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct Color
{
   float r, g, b;
   void apply() const { glColor3f(r, g, b); }
};

namespace COLORS
{
   constexpr Color BLACK = {0, 0, 0};
   constexpr Color WHITE = {1, 1, 1};
   constexpr Color RED = {0.937f, 0.267f, 0.267f};
   constexpr Color GREEN = {0.133f, 0.773f, 0.369f};
   constexpr Color BLUE = {0.055f, 0.647f, 0.914f};
}

namespace Utils
{
   constexpr int WINDOW_WIDTH = 800;
   constexpr int WINDOW_HEIGHT = 600;
}
