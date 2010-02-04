////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-01-30 22:43:41 by Eric Scrivner>
//
// Description:
//   A simple wrapper class for the OIS input system
////////////////////////////////////////////////////////////////////////////////
#ifndef INPUT_SYSTEM_H_
#define INPUT_SYSTEM_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

////////////////////////////////////////////////////////////////////////////////
// Class: InputSystem
//
// Wrapper for the OIS input system
class InputSystem : public OIS::KeyListener, public OIS::MouseListener {
public:
  InputSystem(Ogre::Root* root);
  ~InputSystem();

  //////////////////////////////////////////////////////////////////////////////
  // Function: isKeyDown
  //
  // Returns true if the given key is pressed, false otherwise
  bool isKeyDown(OIS::KeyCode key);

  //////////////////////////////////////////////////////////////////////////////
  // Function: addKeyListener
  //
  // Registers an descendant of OIS::KeyListener with the input system
  void addKeyListener(OIS::KeyListener* listener);

  //////////////////////////////////////////////////////////////////////////////
  // Function: removeKeyListener
  //
  // Removes the indicated object as a keyboard listener
  void removeKeyListener(OIS::KeyListener* listener);

  //////////////////////////////////////////////////////////////////////////////
  // Function: addMouseListener
  //
  // Registers a descendant of OIS::MouseListener with the input system
  void addMouseListener(OIS::MouseListener* listener);

  //////////////////////////////////////////////////////////////////////////////
  // Function: removeMouseListener
  //
  // Removes the indicated object as a mouse listener
  void removeMouseListener(OIS::MouseListener* listener);

  //////////////////////////////////////////////////////////////////////////////
  // Function: update
  //
  // Captures input from all systems with registered listeners
  void update();
private:
  //////////////////////////////////////////////////////////////////////////////
  // Function: createKeyboard
  //
  // Creates a keyboard input system if one does not already exist
  OIS::Keyboard* createKeyboard();

  //////////////////////////////////////////////////////////////////////////////
  // Function: createMouse
  //
  // Creates a mouse input system if one does not already exist
  OIS::Mouse* createMouse();

  //////////////////////////////////////////////////////////////////////////////
  // Function: createInputManager
  //
  // Creates a new OIS input manager given an instance of an Ogre::Root object
  OIS::InputManager* createInputManager(Ogre::Root* root);

  //////////////////////////////////////////////////////////////////////////////
  // OIS::KeyListener overloads
  bool keyPressed(const OIS::KeyEvent& event);
  bool keyReleased(const OIS::KeyEvent& event);

  //////////////////////////////////////////////////////////////////////////////
  // OIS::MouseListener overloads
  bool mouseMoved(const OIS::MouseEvent& event);
  bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id);

  OIS::InputManager*               inputManager_; // The input system
  OIS::Keyboard*                   keyboard_; // The keyboard input system
  OIS::Mouse*                      mouse_; // The mouse input system
  std::vector<OIS::KeyListener*>   keyListeners_; // All key listeners
  std::vector<OIS::MouseListener*> mouseListeners_; // All mouse listeners
};

#endif // INPUT_SYSTEM_H_
