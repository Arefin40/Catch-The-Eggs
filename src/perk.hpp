#pragma once

#pragma region Includes
#include <unordered_map>
#include "entity.hpp"
#pragma endregion

enum class PerkType
{
   BASKET_ENLARGE,
   SLOW_FALL,
   TIME_EXTENSION
};

inline const std::unordered_map<PerkType, Color> PerkColor = {
    {PerkType::BASKET_ENLARGE, COLORS::GREEN},
    {PerkType::SLOW_FALL, COLORS::YELLOW},
    {PerkType::TIME_EXTENSION, COLORS::BLUE},
};

class Perk : public Entity
{
private:
   PerkType type;
   float speed;

public:
   Perk(float x, float y, PerkType type, float speed, float size = Config::PERK::SIZE)
       : Entity(x, y, size, size), type(type), speed(speed) {}

   PerkType getType() const { return type; }
   float getSpeed() const { return speed; }
   void setSpeed(float newSpeed) { speed = newSpeed; }

   void update(float dt) override
   {
      y -= speed * dt;
   }

   void render() override
   {
      float ribbonSize = 6, size = width;
      float cx = x + size / 2, cy = y + size / 2;
      auto color = PerkColor.at(PerkType::BASKET_ENLARGE);

      Draw::rect(x, y, size, size, color.darker(), true);
      Draw::rect(x, y, size, size, color, false);

      Draw::line(x, cy, x + size, cy, COLORS::WHITE);
      Draw::line(cx, y, cx, y + size, COLORS::WHITE);

      Draw::line(cx - ribbonSize, cy - ribbonSize, cx + ribbonSize, cy + ribbonSize, COLORS::WHITE, 2.0f);
      Draw::line(cx - ribbonSize, cy + ribbonSize, cx + ribbonSize, cy - ribbonSize, COLORS::WHITE, 2.0f);
   }
};
