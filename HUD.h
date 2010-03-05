#ifndef HUD_H__
#define HUD_H__

// C++ includes
#include <sstream>
#include <string>

// Other includes
#include "Locator.h"

///////////////////////////////////////////////////////////////////////////////
// CLass: HUD
//
// Manages the heads up display on the screen
class HUD {
public:
  HUD()
    : scoreText_(0),
      healthBar_(0)
  {
    CEGUI::Window* guiSheet = Locator::getGuiSystem()->getGUISheet();

    // Create the score bar
    scoreText_ = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "HUDScore");
    scoreText_->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim(0.02f)));
    scoreText_->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim(0.04f)));
    updateScore(0);
    guiSheet->addChildWindow(scoreText_);
    
    // Create the health bar
    healthBar_ = (CEGUI::ProgressBar*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/ProgressBar",
                                                                                        "HUDHealth");
    healthBar_->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim(0.01f)));
    healthBar_->setSize(CEGUI::UVector2(cegui_reldim(0.107f), cegui_reldim(0.02f)));
    guiSheet->addChildWindow(healthBar_);
  }

  ~HUD() {
    CEGUI::WindowManager::getSingleton().destroyWindow("HUDScore");
    CEGUI::WindowManager::getSingleton().destroyWindow("HUDHealth");
  }

  void updateHealth(const double& healthPercent) {
    healthBar_->setProgress(healthPercent);
  }

  void updateScore(const int& score) {
    std::stringstream ss;
    ss << "Score: " << score;
    scoreText_->setText(ss.str());
  }
private:
  CEGUI::Window* scoreText_; // The text display for the score
  CEGUI::ProgressBar* healthBar_; // The health bar
};

#endif // HUD_H__
