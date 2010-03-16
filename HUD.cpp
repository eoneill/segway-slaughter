#include "HUD.h"

// C++ includes
#include <iomanip>
#include <sstream>
#include <string>

HUD::HUD()
  : scoreText_(0),
    timeLeft_(0),
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
    
    //create chainsaw heat bar
    chainsawHeat_ = (CEGUI::ProgressBar*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/ProgressBar",
                                                                                        "HUDHeat");
    chainsawHeat_->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim(0.05f)));
    chainsawHeat_->setSize(CEGUI::UVector2(cegui_reldim(0.107f), cegui_reldim(0.02f)));
    guiSheet->addChildWindow(chainsawHeat_);

    // Create timer
    timeLeft_ = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "HUDTimer");
    timeLeft_->setPosition(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim(0.01f)));
    timeLeft_->setSize(CEGUI::UVector2(cegui_reldim(0.06f), cegui_reldim(0.04f)));
    updateTimeLeft(0, 0);
    guiSheet->addChildWindow(timeLeft_);
}

///////////////////////////////////////////////////////////////////////////////

HUD::~HUD() {
    CEGUI::WindowManager::getSingleton().destroyWindow("HUDScore");
    CEGUI::WindowManager::getSingleton().destroyWindow("HUDHealth");
    CEGUI::WindowManager::getSingleton().destroyWindow("HUDTimer");
    CEGUI::WindowManager::getSingleton().destroyWindow("HUDHeat");
}

///////////////////////////////////////////////////////////////////////////////

void HUD::updateHealth(const double& healthPercent) {
  healthBar_->setProgress(healthPercent);
}

///////////////////////////////////////////////////////////////////////////////

void HUD::updateHeat(const float& heat){
  chainsawHeat_->setProgress(heat);
}

///////////////////////////////////////////////////////////////////////////////

void HUD::updateScore(const int& score) {
    std::stringstream ss;
    ss << "Score: " << score;
    scoreText_->setText(ss.str());
}

///////////////////////////////////////////////////////////////////////////////

void HUD::updateTimeLeft(const int& mins, const int& secs) {
  std::stringstream ss;
  ss << std::setw(2) << std::setfill('0') << mins << ":";
  ss << std::setw(2) << std::setfill('0') << secs;
  timeLeft_->setText(ss.str());
}
