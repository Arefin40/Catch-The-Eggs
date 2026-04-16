#pragma once

#pragma region Includes
#include "utils.hpp"
#include "renderer.hpp"
#pragma endregion

#define W float(Config::Window::WIDTH)
#define H float(Config::Window::HEIGHT)

namespace Screen
{

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

};
