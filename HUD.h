#ifndef HUD_H__
#define HUD_H__

// Other includes
#include "Locator.h"

///////////////////////////////////////////////////////////////////////////////
// CLass: HUD
//
// Manages the heads up display on the screen
class HUD {
public:
  HUD();
  ~HUD();

  void updateHealth(const double& healthPercent);
  void updateScore(const int& score);
  void updateTimeLeft(const int& mins, const int& secs);
private:
  CEGUI::Window* scoreText_; // The text display for the score
  CEGUI::Window* timeLeft_; // The time left in the level
  CEGUI::ProgressBar* healthBar_; // The health bar
};

#endif // HUD_H__
