#pragma once

#pragma region Includes
#include <vector>
#include <optional>
#include "renderer.hpp"
#include "chicken.hpp"
#include "egg.hpp"
#include "perk.hpp"
#pragma endregion

#pragma region Constants
#define CHICKEN_SIZE Config::CHICKEN::TEX_SIZE
#define W float(Config::Window::WIDTH)

constexpr float BASE_EGG_SPAWN_INTERVAL = 1.5;
constexpr float MIN_EGG_SPAWN_INTERVAL = 0.4;
constexpr float PERK_SPAWN_INTERVAL = 20;
constexpr float BOMB_SPAWN_INTERVAL = 10;
constexpr float EGG_SPAWN_DIFFICULTY_FACTOR = 0.9;
constexpr float BASE_FALL_SPEED = 200;
constexpr float MAX_FALL_SPEED = 480;
constexpr float SLOW_FALL_FACTOR = 0.5f;
#pragma endregion

class Spawner
{
private:
   std::vector<Egg> eggs;
   std::vector<Perk> perks;

   Chicken *normalChicken, *evilChicken;
   float eggSpawnTimer, bombSpawnTimer, perkSpawnTimer;

   std::optional<EggType> e1, e2;
   std::optional<PerkType> p1, p2;

   template <typename T, typename RandFn>
   T selectRandom(std::optional<T> &prev1, std::optional<T> &prev2, RandFn randFn)
   {
      while (true)
      {
         T current = randFn();
         if (prev1.has_value() && prev2.has_value() &&
             prev1.value() == prev2.value() &&
             prev2.value() == current)
            continue;
         prev1 = prev2;
         prev2 = current;
         return current;
      }
   }

   template <typename SpawnFn>
   void tickSpawn(float &timer, float interval, float dt, SpawnFn spawnFn)
   {
      timer += dt;
      if (timer >= interval)
      {
         timer = 0;
         spawnFn();
      }
   }

   template <typename T>
   void updateEntities(std::vector<T> &entities, float dt, float wind)
   {
      for (size_t i = 0; i < entities.size();)
      {
         entities[i].update(dt, wind);
         if (entities[i].getY() + entities[i].getHeight() < 15)
         {
            entities[i] = std::move(entities.back());
            entities.pop_back();
         }
         else
            ++i;
      }
   }

   template <typename T>
   void deleteEntity(std::vector<T> &entities, size_t index)
   {
      if (index < entities.size())
      {
         entities[index] = std::move(entities.back());
         entities.pop_back();
      }
   }

public:
   Spawner() : normalChicken(nullptr), evilChicken(nullptr),
               eggSpawnTimer(0), bombSpawnTimer(0), perkSpawnTimer(0) {}

   ~Spawner() { clear(); }

   const std::vector<Egg> &getEggs() const { return eggs; }
   const std::vector<Perk> &getPerks() const { return perks; }

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
      eggs.clear();
      perks.clear();

      delete normalChicken;
      normalChicken = nullptr;
      delete evilChicken;
      evilChicken = nullptr;

      eggSpawnTimer = bombSpawnTimer = perkSpawnTimer = 0;
   }

   void update(float dt, float wind)
   {
      if (normalChicken)
         normalChicken->update(dt);
      if (evilChicken)
         evilChicken->update(dt);

      tickSpawn(eggSpawnTimer, BASE_EGG_SPAWN_INTERVAL, dt, [&]
                {
         float spawnX = normalChicken->getX() + normalChicken->getSize() / 2 - (Config::EGG::WIDTH / 2);
         float spawnY = normalChicken->getY() - 10;
         eggs.emplace_back(spawnX, spawnY, selectRandom(e1, e2, randomEgg), BASE_FALL_SPEED); });

      tickSpawn(bombSpawnTimer, BOMB_SPAWN_INTERVAL, dt, [&]
                {
         float spawnX = evilChicken->getX() + evilChicken->getSize() / 2 - (Config::EGG::WIDTH / 2);
         float spawnY = evilChicken->getY() - 10;
         eggs.emplace_back(spawnX, spawnY, EggType::BOMB, BASE_FALL_SPEED); });

      tickSpawn(perkSpawnTimer, PERK_SPAWN_INTERVAL, dt, [&]
                {
         float spawnX = Utils::randomFloat(50, W - 50 - Config::PERK::SIZE);
         float spawnY = float(Config::Window::HEIGHT) + Config::PERK::SIZE;
         perks.emplace_back(spawnX, spawnY, selectRandom(p1, p2, randomPerk), BASE_FALL_SPEED * SLOW_FALL_FACTOR); });

      updateEntities(eggs, dt, wind);
      updateEntities(perks, dt, wind);
   }

   void render()
   {
      Draw::stick(Config::STICK::A, 40);
      Draw::stick(Config::STICK::B, 80);

      if (normalChicken)
         normalChicken->render();
      if (evilChicken)
         evilChicken->render();

      for (Egg &egg : eggs)
         egg.render();
      for (Perk &perk : perks)
         perk.render();
   }

   void deleteEgg(size_t index) { deleteEntity(eggs, index); }
   void deletePerk(size_t index) { deleteEntity(perks, index); }
};