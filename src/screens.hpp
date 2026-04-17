#pragma once

#pragma region Includes
#include "utils.hpp"
#include "renderer.hpp"
#pragma endregion

#define W float(Config::Window::WIDTH)
#define H float(Config::Window::HEIGHT)

namespace Screen
{
   inline void HUD(int score = 0, int highScore = 0, float timeRemaining = 9.999)
   {
      float offsetX = 25, offsetY = H - 30;
      bool blinkTime = (static_cast<int>(timeRemaining * 1000.0) / 500) % 2 == 0;

      std::string scoreStr = "SCORE: " + std::to_string(score);
      Text::draw(scoreStr, offsetX, offsetY, COLORS::TEXT::HUD, Font::LG);

      std::string highScoreStr = "HIGH SCORE: " + std::to_string(highScore);
      Text::centered(highScoreStr, offsetY, COLORS::TEXT::HUD, Font::LG);

      std::string timeStr = "TIME: " + Utils::formatTime(timeRemaining);
      float timeStrWidth = Text::width(timeStr, Font::LG);
      Color timeColor = (timeRemaining <= 10 && blinkTime) ? COLORS::RED : COLORS::TEXT::HUD;
      Text::draw(timeStr, W - timeStrWidth - offsetX, offsetY, timeColor, Font::LG);
   }

   inline void MainMenu(int highScore, const Texture *menuChickenTex)
   {
      std::string hiScoreText = "HIGH SCORE RECORD: " + std::to_string(highScore);

      Draw::panel(60);
      menuChickenTex->draw((W - 120) / 2.0, H - 230, 120, 120);
      Text::centered("CATCH THE EGGS", H - 270, COLORS::TEXT::PRIMARY, Font::LG);
      Text::centered(hiScoreText, H - 310, COLORS::YELLOW);
      Text::centered("PRESS ENTER TO PLAY", 210, COLORS::GREEN, Font::LG);
      Text::centered("PRESS 'H' FOR HELP / INSTRUCTIONS", 120, COLORS::BLUE);
      Text::centered("Use Left / Right Arrows or Mouse to control the basket", 90, COLORS::TEXT::MUTED);
   }

   inline void HelpMenu()
   {
      // TODO: Implement help menu screen
   }

   inline void PauseOverlay()
   {
      // TODO: Implement pause overlay
   }

   inline void GameOverOverlay()
   {
      // TODO: Implement game over overlay
   }
};
