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
      float w = Config::Window::WIDTH;
      float h = Config::GRASS::TILE_HEIGHT;

      if (grassTex.isLoaded())
      {
         grassTex.bind();
         COLORS::WHITE.apply();
         float tw = w / Config::GRASS::TILE_WIDTH;

         // clang-format off
         glBegin(GL_QUADS);
         glTexCoord2f(0, 0);  glVertex2f(0, 0);
         glTexCoord2f(tw, 0); glVertex2f(w, 0);
         glTexCoord2f(tw, 1); glVertex2f(w, h);
         glTexCoord2f(0, 1);  glVertex2f(0, h);
         glEnd();
         // clang-format on

         grassTex.unbind();
      }
      else
         Draw::rect(0, 0, w, h, COLORS::GREEN, true);
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