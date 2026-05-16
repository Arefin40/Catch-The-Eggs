#pragma once

#pragma region Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <fstream>
#include "utils.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "screens.hpp"
#include "spawner.hpp"
#include "basket.hpp"
#include "collision.hpp"
#include "audio.hpp"
#pragma endregion

#pragma region Globals
#define ENTER_KEY 13
#define ESC_KEY 27
#define LEFT -1
#define RIGHT 1

struct Airflow
{
   float strength = 0;
   bool wasActive = false;

   static bool isWindActive(float timeRemaining)
   {
      return fmod(timeRemaining, Config::Wind::INTERVAL) < Config::Wind::DURATION;
   }
};

Airflow airflow;
#pragma endregion

enum class GameState
{
   MENU,
   HELP,
   PLAYING,
   PAUSED,
   GAME_OVER
};

class Game
{
private:
   const int GAME_DURATION = Config::Game::DURATION;
   int score, highScore;

   float timeRemaining;
   float wideBasketTimer = 0;
   float slowFallTimer = 0;
   float timeExtensionTimer = 0;
   bool hitBomb = false;
   bool highScoreSoundPlayed = false;

   GameState state;
   Basket basket;
   Spawner spawner;

   Texture menuChickenTex;
   Texture grassTex;
   Texture normalChickenTex;
   Texture evilChickenTex;

public:
   Game() : score(0), highScore(0), timeRemaining(GAME_DURATION),
            state(GameState::MENU) {}

   void loadAssets()
   {
      menuChickenTex.load("assets/textures/menu-chicken.png");
      grassTex.load("assets/textures/grass.png", true);
      normalChickenTex.load("assets/textures/red-chicken.png");
      evilChickenTex.load("assets/textures/black-chicken.png");
   }

   void loadHighScore()
   {
      std::ifstream file("highscore.txt");
      if (file.is_open())
      {
         file >> highScore;
         file.close();
      }
      else
         highScore = 0;
   }

   void init()
   {
      score = 0;
      hitBomb = false;
      wideBasketTimer = 0;
      slowFallTimer = 0;
      timeExtensionTimer = 0;
      timeRemaining = GAME_DURATION;
      Audio::init();
      Audio::initBackgroundMusic();
      Draw::init();
      basket.reset();
      spawner.init(&normalChickenTex, &evilChickenTex);
      loadHighScore();
   }

   void restartGame()
   {
      init();
      state = GameState::PLAYING;
   }

   void endGame(bool byBomb)
   {
      state = GameState::GAME_OVER;
      hitBomb = byBomb;
      Audio::stopMusic();
      Audio::play(byBomb ? SOUND::BOMB : SOUND::GAME_OVER);
   }

   void saveHighScore()
   {
      std::ofstream file("highscore.txt");
      if (file.is_open())
      {
         file << highScore;
         file.close();
      }
   }

   void updateScore(int delta)
   {
      score = std::max(0, score + delta);

      if (score > highScore)
      {
         highScore = score;
         saveHighScore();

         if (!highScoreSoundPlayed)
         {
            Audio::play(SOUND::NEW_HIGH_SCORE);
            highScoreSoundPlayed = true;
         }
      }
   }

   void updateAirflow(float dt)
   {
      bool active = Airflow::isWindActive(timeRemaining);
      static float dir = 1;

      if (active && !airflow.wasActive)
         dir = (rand() % 2 == 0) ? 1 : -1;

      float target = active ? dir * Config::Wind::MAX_STRENGTH : 0;
      airflow.strength += (target - airflow.strength) * Config::Wind::RAMP_SPEED * dt;
      airflow.wasActive = active;
   }

   void handleEggCatch()
   {
      const auto &eggs = spawner.getEggs();
      for (size_t i = 0; i < eggs.size(); ++i)
      {
         Egg egg = eggs[i];
         if (Collision::checkCatch(egg, basket))
         {
            if (egg.getType() == EggType::BOMB)
            {
               endGame(true);
               return;
            }

            switch (egg.getType())
            {
            case EggType::GOLDEN:
               Audio::play(SOUND::GOLDEN_EGG);
               break;
            case EggType::POOP:
               Audio::play(SOUND::POOP);
               break;
            default:
               break;
            }

            updateScore(egg.getScore());
            spawner.deleteEgg(i);
            break;
         }
      }
   }

   void handlePerkCatch()
   {
      const auto &perks = spawner.getPerks();
      for (size_t i = 0; i < perks.size(); ++i)
      {
         Perk perk = perks[i];
         if (Collision::checkCatch(perk, basket))
         {
            switch (perk.getType())
            {
            case PerkType::BASKET_ENLARGE:
               wideBasketTimer = Config::PERK::WIDE_BASKET_DURATION;
               basket.enlarge();
               break;

            case PerkType::SLOW_FALL:
               slowFallTimer = Config::PERK::SLOW_FALL_DURATION;
               break;

            case PerkType::TIME_EXTENSION:
               timeRemaining += Config::PERK::TIME_EXTENSION;
               timeExtensionTimer = Config::PERK::TIME_EXTENSION;
               break;
            }

            Audio::play(SOUND::PERK);
            spawner.deletePerk(i);
            break;
         }
      }
   }

   void update(float dt)
   {
      if (state != GameState::PLAYING)
         return;

      timeRemaining -= dt;
      updateAirflow(dt);

      if (timeRemaining <= 0)
      {
         timeRemaining = 0;
         endGame(false);
         return;
      }

      if (wideBasketTimer > 0)
      {
         wideBasketTimer -= dt;
         if (wideBasketTimer <= 0)
         {
            wideBasketTimer = 0;
            basket.resetSize();
         }
      }

      if (slowFallTimer > 0)
      {
         slowFallTimer -= dt;
         if (slowFallTimer <= 0)
            slowFallTimer = 0;
      }

      if (timeExtensionTimer > 0)
      {
         timeExtensionTimer -= dt;
         if (timeExtensionTimer <= 0)
            timeExtensionTimer = 0;
      }

      basket.update(dt, airflow.strength);
      spawner.update(dt, airflow.strength, GAME_DURATION - timeRemaining, slowFallTimer > 0);
      handleEggCatch();
      handlePerkCatch();
   }

   void render()
   {
      glClear(GL_COLOR_BUFFER_BIT);

      Draw::sky();
      Draw::grass(grassTex);

      if (state == GameState::PLAYING || state == GameState::PAUSED || state == GameState::GAME_OVER)
      {
         spawner.render();
         basket.render();
         Screen::HUD(score, highScore, timeRemaining, wideBasketTimer, slowFallTimer, timeExtensionTimer);
      }

      switch (state)
      {
      case GameState::MENU:
         Screen::MainMenu(highScore, &menuChickenTex);
         break;

      case GameState::HELP:
         Screen::HelpMenu();
         break;

      case GameState::PAUSED:
         Screen::PauseOverlay();
         break;

      case GameState::GAME_OVER:
         Screen::GameOverOverlay(score, highScore, hitBomb);
         break;

      default:
         break;
      }

      glutSwapBuffers();
   }

   void handleKeyDown(unsigned char key, int x, int y)
   {
      switch (state)
      {
      case GameState::MENU:
         if (key == ENTER_KEY)
            restartGame();
         else if (key == 'h' || key == 'H')
            state = GameState::HELP;
         else if (key == ESC_KEY)
            exit(0);
         break;

      case GameState::HELP:
         if (key == ESC_KEY)
            state = GameState::MENU;
         break;

      case GameState::PLAYING:
         if (key == 'p' || key == 'P')
            state = GameState::PAUSED;
         else if (key == ESC_KEY)
            state = GameState::MENU;
         else if (key == 'a' || key == 'A')
            basket.setDirection(LEFT);
         else if (key == 'd' || key == 'D')
            basket.setDirection(RIGHT);
         break;

      case GameState::PAUSED:
         if (key == 'p' || key == 'P')
            state = GameState::PLAYING;
         else if (key == ESC_KEY)
         {
            state = GameState::MENU;
            Audio::initBackgroundMusic();
         }
         break;

      case GameState::GAME_OVER:
         if (key == 'r' || key == 'R')
            restartGame();
         else if (key == ESC_KEY)
         {
            state = GameState::MENU;
            Audio::initBackgroundMusic();
         }
         break;

      default:
         break;
      }
   }

   void handleKeyUp(unsigned char key, int x, int y)
   {
      if (state == GameState::PLAYING && (key == 'a' || key == 'A' || key == 'd' || key == 'D'))
         basket.setDirection(0);
   }

   void handleArrowKeyDown(int key, int x, int y)
   {
      if (state == GameState::PLAYING)
      {
         if (key == GLUT_KEY_LEFT)
            basket.setDirection(LEFT);
         else if (key == GLUT_KEY_RIGHT)
            basket.setDirection(RIGHT);
      }
   }

   void handleArrowKeyUp(int key, int x, int y)
   {
      if (state == GameState::PLAYING && (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT))
         basket.setDirection(0);
   }

   void handleMouseMotion(int x, int y)
   {
      if (state == GameState::PLAYING)
         basket.setCenterX(x);
   }
};