#pragma once

#pragma region Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "utils.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "screens.hpp"
#include "spawner.hpp"
#include "basket.hpp"
#pragma endregion

#pragma region Globals
#define ENTER_KEY 13
#define ESC_KEY 27
#define LEFT -1
#define RIGHT 1
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

   void init()
   {
      score = 0;
      timeRemaining = GAME_DURATION;
      basket.reset();
      spawner.init(&normalChickenTex, &evilChickenTex);
   }

   void restartGame()
   {
      init();
      state = GameState::PLAYING;
   }

   void update(float dt)
   {
      if (state != GameState::PLAYING)
         return;

      timeRemaining -= dt;

      basket.update(dt);
      spawner.update(dt);
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
         Screen::HUD(score, highScore);
      }

      switch (state)
      {
      case GameState::MENU:
         Screen::MainMenu(highScore, &menuChickenTex);
         break;

      case GameState::HELP:
         break;

      case GameState::PAUSED:
         break;

      case GameState::GAME_OVER:
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

      case GameState::GAME_OVER:
         if (key == 'r' || key == 'R')
            restartGame();
         else if (key == ESC_KEY)
            state = GameState::MENU;
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