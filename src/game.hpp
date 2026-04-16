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
#pragma endregion

#pragma region Globals
#define ENTER_KEY 13
#define ESC_KEY 27
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

   GameState state;

   Texture menuChickenTex;
   Texture grassTex;

public:
   Game() : score(0), highScore(0), state(GameState::MENU) {}

   void loadAssets()
   {
      menuChickenTex.load("assets/textures/menu-chicken.png");
      grassTex.load("assets/textures/grass.png", true);
   }

   void init()
   {
      // Reset game state and score
   }

   void restartGame()
   {
      init();
      state = GameState::PLAYING;
   }

   void render()
   {
      glClear(GL_COLOR_BUFFER_BIT);

      Draw::sky();
      Draw::grass(grassTex);

      if (state == GameState::PLAYING || state == GameState::PAUSED || state == GameState::GAME_OVER)
      {
         // TODO: Render HUD and game elements
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

      case GameState::GAME_OVER:
         if (key == 'r' || key == 'R')
            restartGame();
         else if (key == ESC_KEY)
            state = GameState::MENU;
         break;

      default:
         break;
      }
      glutPostRedisplay();
   }

   void handleKeyUp(unsigned char key, int x, int y) {}

   void handleArrowKeyDown(int key, int x, int y)
   {
      if (state == GameState::PLAYING)
      {
         // if (key == MOUSE_LEFT)
         //    move basket left
         // else if (key == MOUSE_RIGHT)
         //    move basket right
      }
   }

   void handleArrowKeyUp(int key, int x, int y)
   {
      if (state == GameState::PLAYING)
      {
         // if (key == MOUSE_LEFT)
         //    stop move basket left
         // else if (key == MOUSE_RIGHT)
         //    stop move basket right
      }
   }

   void handleMouseMotion(int x, int y)
   {
      if (state == GameState::PLAYING)
      {
         // move basket to x position
      }
   }
};