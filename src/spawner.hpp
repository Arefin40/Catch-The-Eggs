#pragma once

#pragma region Includes
#include <vector>
#include "renderer.hpp"
#pragma endregion

class Spawner
{
public:
   void render()
   {
      Draw::stick(Config::STICK::A, 40);
      Draw::stick(Config::STICK::B, 80);
   }
};