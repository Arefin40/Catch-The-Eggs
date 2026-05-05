#pragma once
#include <unordered_map>
#include <algorithm>
#include "entity.hpp"

enum class EggType
{
   STANDARD,
   BLUE,
   GOLDEN,
   POOP,
   BOMB
};

static const std::unordered_map<EggType, int> EggScore = {
    {EggType::STANDARD, 1},
    {EggType::BLUE, 5},
    {EggType::GOLDEN, 10},
    {EggType::POOP, -10},
    {EggType::BOMB, 0}};

inline const std::unordered_map<EggType, Color> EggColor = {
    {EggType::STANDARD, COLORS::WHITE},
    {EggType::BLUE, COLORS::BLUE},
    {EggType::GOLDEN, COLORS::YELLOW},
};

static EggType randomEgg()
{
   int r = Utils::randomInt(1, 100);
   if (r <= 60)
      return EggType::STANDARD;
   else if (r <= 80)
      return EggType::BLUE;
   else if (r <= 90)
      return EggType::GOLDEN;
   else
      return EggType::POOP;
}

class Egg : public Entity
{
private:
   EggType type;
   int score;
   float speed, radiusX, radiusY;

public:
   Egg(float x, float y, EggType type, float speed, float w = Config::EGG::WIDTH, float h = Config::EGG::HEIGHT)
       : Entity(x, y, w, h), type(type), speed(speed)
   {
      radiusX = w / 2;
      radiusY = h / 2;
      score = EggScore.at(type);
   }

   EggType getType() const { return type; }
   float getSpeed() const { return speed; }
   void setSpeed(float newSpeed) { speed = newSpeed; }
   int getScore() const { return score; }

   void update(float dt, float wind) override
   {
      x = std::clamp(x + wind * dt, 50.0f, float(Config::Window::WIDTH) - width - 50);

      y -= speed * dt;
   }

   void render() override
   {
      float cx = x + radiusX;
      float cy = y + radiusY;

      if (type == EggType::POOP)
         Draw::poop(cx, cy);
      else if (type == EggType::BOMB)
         Draw::bomb(cx, cy, radiusX, radiusY, COLORS::BLACK);
      else
         Draw::egg(cx, cy, radiusX, radiusY, EggColor.at(type));
   }
};
