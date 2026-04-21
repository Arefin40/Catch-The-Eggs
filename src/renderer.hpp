#pragma once

#pragma region Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <vector>
#include "utils.hpp"
#include "texture.hpp"
#pragma endregion

#define W float(Config::Window::WIDTH)
#define H float(Config::Window::HEIGHT)

class CircleCache
{
public:
   std::vector<std::pair<float, float>> unitCircle;

   CircleCache(int segments = 32)
   {
      unitCircle.reserve(segments);

      for (int i = 0; i < segments; ++i)
      {
         float theta = 2 * 3.1415926f * i / segments;
         unitCircle.emplace_back(std::cos(theta), std::sin(theta));
      }
   }
};

namespace Draw
{
   inline CircleCache *circleCache = nullptr;

   inline void init()
   {
      circleCache = new CircleCache(32);
   }

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

   inline void ellipse(float cx, float cy, float rx, float ry, const Color &color, bool filled = true)
   {
      color.apply();
      const auto &pts = circleCache->unitCircle;
      int n = pts.size();

      glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
      for (int i = 0; i < n; i++)
         glVertex2f(cx + pts[i].first * rx, cy + pts[i].second * ry);
      glEnd();
   }

   inline void circle(float cx, float cy, float radius, const Color &color, bool filled = true)
   {
      ellipse(cx, cy, radius, radius, color, filled);
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

   inline void chicken(float x, float y, float size)
   {
      const Color body = {0.937f, 0.878f, 0.757f};
      const Color wing = {0.855f, 0.761f, 0.600f};
      const Color feather = {0.922f, 0.859f, 0.725f};

      const float cx = x + size * 0.5f;
      const float cy = y + size * 0.5f - 9.0f;

      // Beak
      COLORS::YELLOW.apply();
      glBegin(GL_TRIANGLES);
      glVertex2f(cx - 33, cy + 18);
      glVertex2f(cx - 27, cy + 14);
      glVertex2f(cx - 27, cy + 20);
      glEnd();

      // Tail feathers
      feather.apply();

      const float tail[][6] = {
          {15, -1, 30, -6, 23, -13},
          {15, 1, 31, 7, 31, -4},
          {15, 3, 29, 8, 22, 14}};

      glBegin(GL_TRIANGLES);
      for (const auto &t : tail)
      {
         glVertex2f(cx + t[0], cy + t[1]);
         glVertex2f(cx + t[2], cy + t[3]);
         glVertex2f(cx + t[4], cy + t[5]);
      }
      glEnd();

      // Comb
      Draw::circle(cx - 19.5f, cy + 30.5f, 4.5f, COLORS::RED, true);
      Draw::circle(cx - 13.0f, cy + 29.0f, 4.0f, COLORS::RED, true);
      Draw::circle(cx - 8.0f, cy + 27.0f, 3.0f, COLORS::RED, true);

      // Body
      Draw::circle(cx - 17, cy + 17, 11, body, true);
      Draw::circle(cx, cy + 4, 18, body, true);
      Draw::ellipse(cx, cy + 4, 12, 7.5, wing, true);
      Draw::circle(cx - 21, cy + 21, 1.5, COLORS::BLACK, true);

      // Legs
      Draw::rect(cx - 7, cy - 22, 3, 10, COLORS::ORANGE, true);
      Draw::rect(cx + 4, cy - 22, 3, 10, COLORS::ORANGE, true);

      COLORS::ORANGE.apply();
      glBegin(GL_TRIANGLES);

      glVertex2f(cx - 5, cy - 21);
      glVertex2f(cx - 2, cy - 26);
      glVertex2f(cx - 10, cy - 26);

      glVertex2f(cx + 6, cy - 21);
      glVertex2f(cx + 1, cy - 26);
      glVertex2f(cx + 9, cy - 26);

      glEnd();
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