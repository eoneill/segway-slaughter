////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   The main menu of the game
////////////////////////////////////////////////////////////////////////////////
#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

#include "GameState.h"
#include "SideScroller.h"
#include <iostream>
#include <CEGUI/elements/CEGUIPushButton.h>

class MainMenu : public GameState, public OIS::MouseListener {
public:
  MainMenu();
  ~MainMenu();
  void initialize();
  GameState* update();
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

  bool mouseMoved(const OIS::MouseEvent& event) {
    Locator::getGuiSystem()->injectMouseMove(event.state.X.rel,
                                             event.state.Y.rel);
    return true;
  }

  bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id) {
    std::cout << "Mouse press\n";
    std::cout.flush();
    CEGUI::MouseButton button = CEGUI::NoButton;
    switch(id) {
    case OIS::MB_Left:
      button = CEGUI::LeftButton;
      break;
    case OIS::MB_Middle:
      button = CEGUI::MiddleButton;
      break;
    case OIS::MB_Right:
      button = CEGUI::RightButton;
      break;
    default: break;
    }

    Locator::getGuiSystem()->injectMouseButtonDown(button);
    return true;
  }

  bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id) {
    CEGUI::MouseButton button = CEGUI::NoButton;
    switch(id) {
    case OIS::MB_Left:
      button = CEGUI::LeftButton;
      break;
    case OIS::MB_Middle:
      button = CEGUI::MiddleButton;
      break;
    case OIS::MB_Right:
      button = CEGUI::RightButton;
      break;
    default: break;
    }

    Locator::getGuiSystem()->injectMouseButtonUp(button);
    return true;
  }
private:
  CEGUI::PushButton* makeButton(const std::string& resourcePath,
                                const std::string& buttonName);
  bool isDone_;
  GameState* next_;
};

#endif // MAIN_MENU_H_
