////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   The main menu of the game
////////////////////////////////////////////////////////////////////////////////
#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

#include "GameState.h"
#include "GuiListener.h"
#include "SideScroller.h"
#include <iostream>
#include <CEGUI/elements/CEGUIPushButton.h>

class MainMenu : public GameState, public GuiListener {
public:
  MainMenu();
  ~MainMenu();
  void initialize();
  GameState* update(const Ogre::Real& timeSinceLastFrame);
  bool isDone();

  bool onQuit(const CEGUI::EventArgs& e) {
    isDone_ = true;
    return true;
  }

  bool onStart(const CEGUI::EventArgs& e) {
    next_ = new SideScroller();
    isDone_ = true;
    return true;
  }

  bool onOptions(const CEGUI::EventArgs& e) {
    return true;
  }
private:
  CEGUI::PushButton* makeButton(const std::string& resourcePath,
                                const std::string& buttonName);
  bool isDone_;
  GameState* next_;
};

#endif // MAIN_MENU_H_
