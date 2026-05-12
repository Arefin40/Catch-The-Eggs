#pragma once

#pragma region Includes
#include <cstdio>
#include "utils.hpp"
#include "renderer.hpp"
#pragma endregion

#define W float(Config::Window::WIDTH)
#define H float(Config::Window::HEIGHT)

namespace Screen
{
   inline void HUD(int score, int highScore, float timeRemaining,
                   float wideBasketTimer, float slowFallTimer, float timeExtensionTimer)
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

      offsetY -= 30;

      if (timeExtensionTimer > 0)
      {
         std::string str = "[+] TIME EXTENSION: " + std::to_string(int(Config::PERK::TIME_EXTENSION)) + "s";
         float strWidth = Text::width(str);
         Text::draw(str, W - strWidth - offsetX, offsetY, COLORS::BLUE.luma(0.75));
      }

      if (wideBasketTimer > 0)
      {
         char buf[64];
         std::snprintf(buf, sizeof(buf), "[+] WIDE BASKET: %.1f s", wideBasketTimer);
         Text::draw(std::string(buf), offsetX, offsetY, COLORS::GREEN.luma(0.75));
         offsetY -= 20;
      }

      if (slowFallTimer > 0)
      {
         char buf[64];
         std::snprintf(buf, sizeof(buf), "[-v] SLOW EGGS: %.1f s", slowFallTimer);
         Text::draw(std::string(buf), offsetX, offsetY, COLORS::YELLOW.luma(0.75));
      }
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
      Draw::panel(50);

      Text::centered("GAME INSTRUCTIONS", H - 95, COLORS::TEXT::PRIMARY, Font::LG);

      const float leftX = 80.0f;
      const float rightX = W / 2.0f + 30.0f;
      const float lineGap = 26.0f;
      float yOffset = H - 140.0f;

      Text::draw("EGG SCORING VALUES:", leftX, yOffset, COLORS::BLUE);
      yOffset -= lineGap;

      auto drawEggEntry = [&](const std::string &label, const std::string &desc, const Color &color)
      {
         Draw::ellipse(leftX + 14.0f, yOffset + 7.0f, 7.0f, 10.0f, color, true);
         Draw::circle(leftX + 11.5f, yOffset + 10.5f, 2.2f, color.lighter(), true);
         Text::draw(label, leftX + 30.0f, yOffset, COLORS::TEXT::PRIMARY);
         Text::draw(" : " + desc, leftX + 130.0f, yOffset, COLORS::TEXT::MUTED);
         yOffset -= lineGap;
      };

      drawEggEntry("Golden Egg", "+10 Points", COLORS::YELLOW);
      drawEggEntry("Blue Egg", "+5 Points", COLORS::BLUE);
      drawEggEntry("Normal Egg", "+1 Point", COLORS::WHITE);
      drawEggEntry("Poop", "-10 Points", COLORS::BROWN);
      drawEggEntry("Black Egg", "INSTANT GAME OVER!", Color{0.12f, 0.12f, 0.12f});

      yOffset = H - 140.0f;

      Text::draw("POWER-UPS / PERKS:", rightX, yOffset, COLORS::GREEN);
      yOffset -= lineGap;

      auto drawPerkEntry = [&](const std::string &desc, const Color &color)
      {
         Draw::rect(static_cast<int>(rightX + 5.0f), static_cast<int>(yOffset - 1.0f), 12.0f, 12.0f, color, true);
         Text::draw(" : " + desc, rightX + 24.0f, yOffset, COLORS::TEXT::PRIMARY);
         yOffset -= lineGap;
      };

      drawPerkEntry("Temporarily widens basket", COLORS::BLUE);
      drawPerkEntry("Slows down egg speed by 50%", COLORS::YELLOW);
      drawPerkEntry("Adds +5 seconds of time", COLORS::GREEN);

      yOffset -= 20.0f;

      Text::draw("INPUT CONTROLS:", rightX, yOffset, COLORS::ORANGE);
      yOffset -= lineGap;

      auto drawControlEntry = [&](const std::string &desc)
      {
         Text::draw(desc, rightX, yOffset, COLORS::TEXT::PRIMARY);
         yOffset -= lineGap;
      };

      drawControlEntry("Keyboard: A / D or Left / Right Arrow keys");
      drawControlEntry("Mouse: Tracks mouse X coordinate directly");
      drawControlEntry("Game Toggles: 'P' to Pause, 'ESC' to exit");

      Text::centered("PRESS 'ESC' TO RETURN TO MAIN MENU", 85, COLORS::GREEN);
   }

   inline void PauseOverlay()
   {
      Draw::rect(0.0f, 0.0f, W, H, COLORS::BLACK, 0.5f);
      Draw::panel(220);
      Text::centered("GAME PAUSED", H / 2.0f + 15.0f, COLORS::TEXT::PRIMARY, Font::LG);
      Text::centered("Press 'P' to Resume", H / 2.0f - 25.0f, COLORS::TEXT::MUTED);
      Text::centered("Press 'ESC' for Main Menu", H / 2.0f - 55.0f, COLORS::TEXT::MUTED);
   }

   inline void GameOverOverlay(int finalScore, int highScore, bool hitBomb)
   {
      Draw::rect(0.0f, 0.0f, W, H, COLORS::BLACK, 0.75f);
      Draw::rect(180.0f, 140.0f, W - 360.0f, H - 280.0f, Color{0.25f, 0.08f, 0.08f}, 0.95f);
      Draw::rect(180, 140, W - 360.0f, H - 280.0f, Color{0.95f, 0.1f, 0.1f}, false);

      Text::centered(hitBomb ? "BOMB CAUGHT! INSTANT DEATH" : "TIME OUT!", H - 190.0f, COLORS::RED, Font::LG);
      Text::centered("GAME OVER", H - 230.0f, COLORS::TEXT::PRIMARY);

      std::string finText = "Final Score: " + std::to_string(finalScore);
      Text::centered(finText, H - 280.0f, COLORS::YELLOW, Font::LG);

      std::string highText = "High Score: " + std::to_string(highScore);
      Text::centered(highText, H - 320.0f, COLORS::YELLOW);

      Text::centered("Press 'R' to Restart Game", 165.0f, COLORS::TEXT::PRIMARY);
      Text::centered("Press 'ESC' for Main Menu", 130.0f, COLORS::TEXT::MUTED);
   }
};
