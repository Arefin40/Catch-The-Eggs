#pragma once

#include <algorithm>
#include "entity.hpp"
#include "utils.hpp"
#include "renderer.hpp"

#define W float(Config::Window::WIDTH)
#define H float(Config::Window::HEIGHT)
#define BW Config::BASKET::WIDTH
#define BH Config::BASKET::HEIGHT
#define BEW Config::BASKET::ENLARGED_WIDTH

class Basket : public Entity
{
private:
   float vx;
   float accel;
   float decel;
   float maxSpeed;
   int direction;

public:
   Basket(float w = BW, float h = BH)
       : Entity((W - w) / 2.0f, 0.0f, w, h),
         vx(0),
         accel(2400),
         decel(1800),
         maxSpeed(700),
         direction(0) {}

   void setDirectionToLeft() { direction = -1; }
   void setDirectionToRight() { direction = 1; }
   void setDirection(int direction) { this->direction = direction; }

   void setCenterX(float x)
   {
      this->x = std::clamp(x - width / 2, 0.0f, W - width);
   }

   void reset()
   {
      x = (W - width) / 2.0f;
      vx = 0;
      direction = 0;
   }

   void enlarge()
   {
      width = BW + BEW;
      x = std::max(0.0f, x - (BEW / 2));
   }

   void resetSize()
   {
      width = BW;
      x += BEW / 2;
   }

   void update(float dt, float wind) override
   {

      if (direction == -1)
         vx -= accel * dt;
      else if (direction == 1)
         vx += accel * dt;
      else
         vx = vx > 0
                  ? std::max(0.0f, vx - decel * dt)
                  : std::min(0.0f, vx + decel * dt);

      vx = std::clamp(vx, -maxSpeed, maxSpeed);

      x += vx * dt;
      if (x < 0)
         x = vx = 0;
      else if (x > (W - width))
         x = W - width, vx = 0;
   }

   void render() override
   {
      Draw::basket(x, y, width, height, COLORS::BROWN);
   }
};
