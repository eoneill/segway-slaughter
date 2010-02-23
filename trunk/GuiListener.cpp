////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Encapsulates mouse movement and button press injection for CEGUI.
////////////////////////////////////////////////////////////////////////////////
#include "GuiListener.h"
#include "Locator.h"

GuiListener::GuiListener() { 
  Locator::getInput()->addMouseListener(this);
}

////////////////////////////////////////////////////////////////////////////////

bool GuiListener::mouseMoved(const OIS::MouseEvent& event) {
  Locator::getGuiSystem()->injectMouseMove(event.state.X.rel,
                                           event.state.Y.rel);
  return true;
}

////////////////////////////////////////////////////////////////////////////////

bool GuiListener::mousePressed(const OIS::MouseEvent& event,
                               OIS::MouseButtonID id) {
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

////////////////////////////////////////////////////////////////////////////////

bool GuiListener::mouseReleased(const OIS::MouseEvent& event,
                                OIS::MouseButtonID id) {
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
