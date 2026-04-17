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

   inline void stick(float y, int segment_start = 40)
   {
      float r = 0.44f, g = 0.5f, b = 0.16f;
      Color c1 = {r, g, b};
      Color c2 = {r * 0.6f, g * 0.6f, b * 0.6f};

      rect(0, y, W, 8, c1, true);
      rect(0, y, W, 8, c2, false);

      for (int x = segment_start; x < W; x += 80)
         rect(x, y - 2, 3, 12, c2, true);
   }

   inline void basket(float x, float y, float w, float h, const Color &color)
   {
      float top = y + h * 0.85f, padding = w * 0.05f;

      color.apply();
      glBegin(GL_QUADS);
      glVertex2f(x, top);
      glVertex2f(x + w, top);
      glVertex2f(x + w * 0.88f, y);
      glVertex2f(x + w * 0.12f, y);
      glEnd();

      color.applyf(0.85);
      glBegin(GL_QUADS);
      glVertex2f(x - padding, top);
      glVertex2f(x + w + padding, top);
      glVertex2f(x + w + padding, y + h);
      glVertex2f(x - padding, y + h);
      glEnd();

      color.applyf(0.6);
      glBegin(GL_LINES);
      for (int i = 1; i <= 6; ++i)
      {
         float t = float(i) / 7.0f;
         glVertex2f(x + w * (0.12f + 0.76f * t), y);
         glVertex2f(x + w * t, top);
      }
      for (int i = 1; i < 4; ++i)
      {
         float t = float(i) / 4.0f;
         float leftX = x + w * (0.12f * (1.0f - t));
         float rightX = x + w * (0.88f + 0.12f * t);
         float currY = top * t;
         glVertex2f(leftX, currY);
         glVertex2f(rightX, currY);
      }
      glEnd();

      glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
      glBegin(GL_LINES);
      glVertex2f(x - padding, y + h * 0.95f);
      glVertex2f(x + w + padding, y + h * 0.95f);
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

   inline void panel(int padding)
   {
      int px2 = 2 * padding;
      rect(padding, padding, W - px2, H - px2, COLORS::Panel::FILL, true);
      rect(padding, padding, W - px2, H - px2, COLORS::Panel::BORDER, false);
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