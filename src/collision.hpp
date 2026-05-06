#pragma once

#pragma region Includes
#include "entity.hpp"
#include "egg.hpp"
#include "perk.hpp"
#pragma endregion

constexpr float CATCH_TOLERANCE = 4.0f;

namespace Collision
{
   inline bool checkCatch(const Entity &item, const Entity &basket)
   {
      bool withinBasketXBoundary = (item.getX() >= basket.getX() + CATCH_TOLERANCE) &&
                                   (item.getX() + item.getWidth() <=
                                    basket.getX() + basket.getWidth() - CATCH_TOLERANCE);

      float basketTopRim = basket.getY() + basket.getHeight() - 5;
      float itemBottom = item.getY();

      bool touchedBasketRim = (itemBottom >= basketTopRim - CATCH_TOLERANCE) &&
                              (itemBottom <= basketTopRim + CATCH_TOLERANCE);

      return withinBasketXBoundary && touchedBasketRim;
   }
}