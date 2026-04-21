#pragma once

#pragma region Includes
#include <vector>
#include "renderer.hpp"
#include "chicken.hpp"
#pragma endregion

#define STICK1_Y Config::STICK::A
#define STICK2_Y Config::STICK::B
#define CHICKEN_SIZE Config::CHICKEN::TEX_SIZE
#define W float(Config::Window::WIDTH)

class Spawner
{
private:
   Chicken *normalChicken, *evilChicken;

public:
   Spawner() : normalChicken(nullptr), evilChicken(nullptr) {}
   ~Spawner() { clear(); }

   void init(const Texture *normalTex, const Texture *evilTex)
   {
      clear();
      int padding = Config::CHICKEN::MOVEMENT_PADDING;

      normalChicken = new Chicken(padding, Config::STICK::A + 8,
                                  Config::CHICKEN::NORMAL_SPEED, normalTex);

      evilChicken = new Chicken(W - CHICKEN_SIZE - padding, Config::STICK::B + 8,
                                Config::CHICKEN::EVIL_SPEED, evilTex);
   }

   void clear()
   {
      if (normalChicken)
      {
         delete normalChicken;
         normalChicken = nullptr;
      }
      if (evilChicken)
      {
         delete evilChicken;
         evilChicken = nullptr;
      }
   }

   void update(float dt)
   {
      if (normalChicken)
         normalChicken->update(dt);
      if (evilChicken)
         evilChicken->update(dt);
   }

   void render()
   {
      Draw::stick(Config::STICK::A, 40);
      Draw::stick(Config::STICK::B, 80);

      if (normalChicken)
         normalChicken->render();

      if (evilChicken)
         evilChicken->render();
   }
};