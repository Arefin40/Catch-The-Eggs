#pragma once

#include "utils.hpp"
#include "renderer.hpp"

class Entity
{
protected:
   float x, y;
   float width, height;
   bool active;

   void renderBoundingBox() const
   {
      Draw::rect(x, y, width, height, COLORS::RED, false);

      float cx = x + width / 2;
      float cy = y + height / 2;

      glPointSize(5);
      glBegin(GL_POINTS);
      COLORS::BLACK.apply();
      glVertex2f(x, y);
      glVertex2f(cx, cy);
      glEnd();
      glPointSize(1);
   }

public:
   Entity(float x, float y, float width, float height)
       : x(x), y(y), width(width), height(height), active(true) {}

   virtual ~Entity() = default;
   virtual void update(float dt) = 0;
   virtual void render() = 0;

   float getX() const { return x; }
   void setX(float newX) { x = newX; }

   float getY() const { return y; }
   void setY(float newY) { y = newY; }

   float getWidth() const { return width; }
   void setWidth(float newW) { width = newW; }

   float getHeight() const { return height; }
   void setHeight(float newH) { height = newH; }

   bool isActive() const { return active; }
   void setActive(bool state) { active = state; }
};
