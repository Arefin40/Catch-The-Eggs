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

      glutSwapBuffers();
   }
};