////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Encapsulates mouse movement and button press injection for CEGUI.
////////////////////////////////////////////////////////////////////////////////
#ifndef GUI_LISTENER_H_
#define GUI_LISTENER_H_

#include <OIS/OIS.h>

////////////////////////////////////////////////////////////////////////////////
// Class: GuiListener
//
// Injects mouse clicks into the CEGUI system
class GuiListener : public OIS::MouseListener {
public:
  GuiListener();

  virtual ~GuiListener()
  { } 

  bool mouseMoved(const OIS::MouseEvent& event);
  bool mousePressed(const OIS::MouseEvent& event,
                    OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent& event,
                     OIS::MouseButtonID id);
};

#endif // GUI_LISTENER_H_
