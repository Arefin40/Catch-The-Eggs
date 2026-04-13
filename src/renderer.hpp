#pragma once

#pragma region Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "utils.hpp"
#include "texture.hpp"
#pragma endregion

#define W float(Config::Window::WIDTH)
#define H float(Config::Window::HEIGHT)

namespace Draw
{
   inline void rect(int x, int y, float width, float height, const Color &color, bool filled = true)
   {
      color.apply();
      glBegin(filled ? GL_QUADS : GL_LINE_LOOP);
      glVertex2i(x, y);
      glVertex2i(x + width, y);
      glVertex2i(x + width, y + height);
      glVertex2i(x, y + height);
      glEnd();
   }

   inline void grass(const Texture &grassTex)
   {
      float h = Config::GRASS::TILE_HEIGHT;

      if (grassTex.isLoaded())
      {
         float tw = W / Config::GRASS::TILE_WIDTH;
         grassTex.draw(0, 0, W, h, 0, 0, tw, 1);
      }
      else
         Draw::rect(0, 0, W, h, COLORS::GREEN, true);
   }

   inline void sky()
   {
      glBegin(GL_QUADS);
      COLORS::SKY::TOP.apply();
      glVertex2f(0, Config::Window::HEIGHT);
      glVertex2f(Config::Window::WIDTH, Config::Window::HEIGHT);

      COLORS::SKY::BOTTOM.apply();
      glVertex2f(Config::Window::WIDTH, 0);
      glVertex2f(0, 0);
      glEnd();
   }
}

namespace Text
{
   inline int width(const std::string &text, void *font = Font::SM)
   {
      int w = 0;
      for (char c : text)
         w += glutBitmapWidth(font, c);
      return w;
   }

   inline void draw(const std::string &text, float x, float y, const Color &color, void *font = Font::SM)
   {
      color.apply();
      glRasterPos2f(x, y);
      for (char c : text)
         glutBitmapCharacter(font, c);
   }

   inline void centered(const std::string &text, float y, const Color &color, void *font = Font::SM)
   {
      float cx = (W - width(text, font)) / 2.0f;
      draw(text, cx, y, color, font);
   }
}