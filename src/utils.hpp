#pragma once

#pragma region Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#pragma endregion

namespace Font
{
   constexpr void *SM = GLUT_BITMAP_HELVETICA_12;
   constexpr void *LG = GLUT_BITMAP_HELVETICA_18;
}

struct Color
{
   float r, g, b;
   constexpr Color(float r, float g, float b) : r(r), g(g), b(b) {}

   constexpr Color luma(float f) const { return {r * f, g * f, b * f}; }
   constexpr Color lighter() const { return luma(1.3f); }
   constexpr Color darker() const { return luma(0.7f); }

   void apply() const { glColor3f(r, g, b); }
   void applyf(float f) const { luma(f).apply(); }
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
   constexpr Color BROWN = {0.58f, 0.38f, 0.22f};

   namespace TEXT
   {
      constexpr Color PRIMARY = {0.95f, 0.95f, 0.95f};
      constexpr Color MUTED = {0.612f, 0.639f, 0.686f};
      constexpr Color HUD = BLUE.darker();
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
   constexpr bool SHOW_BOUNDING_BOX = true;

   namespace Window
   {
      constexpr int WIDTH = 800;
      constexpr int HEIGHT = 600;
   }

   namespace Game
   {
      constexpr int DURATION = 120;
   }

   namespace Wind
   {
      constexpr float INTERVAL = 30;
      constexpr float DURATION = 10;
      constexpr float MAX_STRENGTH = 70;
      constexpr float RAMP_SPEED = 15;
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

   namespace STICK
   {
      constexpr float A = 480;
      constexpr float B = 380;
   }

   namespace BASKET
   {
      constexpr float WIDTH = 110;
      constexpr float HEIGHT = 60;
      constexpr float ENLARGED_WIDTH = 70;
   }

   namespace CHICKEN
   {
      constexpr float MOVEMENT_PADDING = 50;
      constexpr float TEX_SIZE = 70;
      constexpr float NORMAL_SPEED = 140;
      constexpr float EVIL_SPEED = 90;
   }

   namespace EGG
   {
      constexpr float WIDTH = 26;
      constexpr float HEIGHT = 34;
   }

   namespace PERK
   {
      constexpr float SIZE = 32;
      constexpr float WIDE_BASKET_DURATION = 10;
      constexpr float SLOW_FALL_DURATION = 5;
      constexpr float TIME_EXTENSION = 5;
   }

}

namespace Utils
{
   inline int randomInt(int min, int max)
   {
      static std::random_device rd;
      static std::mt19937 gen(rd());
      std::uniform_int_distribution<int> dis(min, max);
      return dis(gen);
   }

   inline float randomFloat(float min, float max)
   {
      static std::random_device rd;
      static std::mt19937 gen(rd());
      std::uniform_real_distribution<float> dis(min, max);
      return dis(gen);
   }

   inline std::string formatTime(double seconds)
   {
      int mins = static_cast<int>(seconds) / 60;
      int secs = static_cast<int>(seconds) % 60;

      std::stringstream ss;
      ss << std::setfill('0')
         << std::setw(2) << mins << ":"
         << std::setw(2) << secs;

      return ss.str();
   }
}