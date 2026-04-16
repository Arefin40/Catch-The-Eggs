#pragma once

#pragma region Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#pragma endregion

namespace Font
{
   constexpr void *SM = GLUT_BITMAP_HELVETICA_12;
   constexpr void *LG = GLUT_BITMAP_HELVETICA_18;
}

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
   constexpr Color YELLOW = {0.984f, 0.749f, 0.141f};
   constexpr Color ORANGE = {0.984f, 0.573f, 0.235f};
   constexpr Color TEAL = {0.078f, 0.722f, 0.651f};
   constexpr Color PINK = {0.925f, 0.282f, 0.600f};

   namespace TEXT
   {
      constexpr Color PRIMARY = {0.95f, 0.95f, 0.95f};
      constexpr Color MUTED = {0.612f, 0.639f, 0.686f};
   }

   namespace SKY
   {
      constexpr Color TOP = {0.53f, 0.81f, 0.98f};
      constexpr Color BOTTOM = {0.85f, 0.95f, 1.0f};
   }

   namespace Panel
   {
      constexpr Color FILL = {0.067f, 0.094f, 0.153f};
      constexpr Color BORDER = {0.216f, 0.255f, 0.318f};
   }
}

namespace Config
{
   namespace Window
   {
      constexpr int WIDTH = 800;
      constexpr int HEIGHT = 600;
   }

   namespace Game
   {
      constexpr int DURATION = 120;
   }

   namespace UI
   {
      constexpr float LINE_WIDTH = 1.5;
   }

   namespace GRASS
   {
      constexpr float TILE_WIDTH = 169;
      constexpr float TILE_HEIGHT = 41;
   }

}
