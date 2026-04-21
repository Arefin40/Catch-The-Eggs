#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "texture.hpp"
#include "utils.hpp"
#include "renderer.hpp"

using namespace std;

class Chicken
{
private:
   float x, y;
   float size;
   float speed;
   bool movingRight;
   const Texture *texture;

public:
   Chicken(float x, float y, float speed, const Texture *tex, float size = Config::CHICKEN::TEX_SIZE)
       : x(x), y(y), size(size), speed(speed), movingRight(true), texture(tex) {}

   float getX() const { return x; }
   float getY() const { return y; }
   float getSize() const { return size; }
   bool isMovingRight() const { return movingRight; }

   void update(float dt)
   {
      float minX = 15;
      float maxX = float(Config::Window::WIDTH) - size - 15;
      x += movingRight ? (speed * dt) : -(speed * dt);

      if (x <= minX)
         x = minX, movingRight = true;
      else if (x >= maxX)
         x = maxX, movingRight = false;
   }

   void render() const
   {
      if (texture && texture->isLoaded())
      {
         float u0 = movingRight ? 0 : 1;
         float u1 = movingRight ? 1 : 0;
         texture->draw(x, y, size, size, u0, 0, u1, 1);
      }
      else
         Draw::chicken(x, y, size);
   }
};
