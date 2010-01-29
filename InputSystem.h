////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-01-29 14:48:08 by Eric Scrivner>
//
// Description:
//   A simple wrapper class for the OIS input system
////////////////////////////////////////////////////////////////////////////////
#ifndef INPUT_SYSTEM_H_
#define INPUT_SYSTEM_H_

#include <Ogre.h>
#include <OIS/OIS.h>

////////////////////////////////////////////////////////////////////////////////
// Class: InputSystem
//
// Wrapper for the OIS input system
class InputSystem {
public:
  InputSystem(Ogre::Root* root);
  ~InputSystem();

  //////////////////////////////////////////////////////////////////////////////
  // Function: addKeyListener
  //
  // Registers an descendant of OIS::KeyListener with the input system
  void addKeyListener(OIS::KeyListener* listener);

  //////////////////////////////////////////////////////////////////////////////
  // Function: addMouseListener
  //
  // Registers a descendant of OIS::MouseListener with the input system
  void addMouseListener(OIS::MouseListener* listener);

  //////////////////////////////////////////////////////////////////////////////
  // Function: update
  //
  // Captures input from all systems with registered listeners
  void update();
private:
  //////////////////////////////////////////////////////////////////////////////
  // Function: createInputManager
  //
  // Creates a new OIS input manager given an instance of an Ogre::Root object
  OIS::InputManager* createInputManager(Ogre::Root* root);

  OIS::InputManager* inputManager_; // The input system
  OIS::Keyboard*     keyboard_; // The keyboard input system
  OIS::Mouse*        mouse_; // The mouse input system
};

#endif // INPUT_SYSTEM_H_
