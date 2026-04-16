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

   void handleKeyDown(unsigned char key, int x, int y) {}

   void handleKeyUp(unsigned char key, int x, int y) {}

   void handleArrowKeyDown(int key, int x, int y) {}

   void handleArrowKeyUp(int key, int x, int y) {}

   void handleMouseMotion(int x, int y) {}
};