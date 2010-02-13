////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Implements to locator pattern for providing a central place for resource
// access.
////////////////////////////////////////////////////////////////////////////////
#ifndef LOCATOR_H_
#define LOCATOR_H_

#include <CEGUI/CEGUI.h>
#include "InputSystem.h"

////////////////////////////////////////////////////////////////////////////////
// Class: Locator
//
// Provides a central location for access to common resources
class Locator {
public:
  //////////////////////////////////////////////////////////////////////////////
  // Function: getInput
  //
  // Gets a pointer to the current input system
  static InputSystem* getInput() {
    assert(inputSystem_ != 0);
    return inputSystem_;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: registerInput
  //
  // Registers an input system to be returned by the locator
  static void registerInput(InputSystem* input) {
    inputSystem_ = input;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: registerRoot
  //
  // Registers an Ogre root object
  static void registerRoot(Ogre::Root* root) {
    root_ = root;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: getRoot
  //
  // Returns the current instance of the Ogre root object
  static Ogre::Root* getRoot() {
    assert(root_ != 0);
    return root_;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: registerGuiSystem
  //
  // Registers a CEGUI::System object
  static void registerGuiSystem(CEGUI::System* system) {
    ceguiSystem_ = system;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: getGuiSystem
  //
  // Returns the current instance of the CEGUI::System object
  static CEGUI::System* getGuiSystem() {
    assert(ceguiSystem_ != 0);
    return ceguiSystem_;
  }
private:
  static InputSystem*   inputSystem_; // The input system instance
  static Ogre::Root*    root_;  // The Ogre root object
  static CEGUI::System* ceguiSystem_; // The CEGUI system object
};

#endif // LOCATOR_H_
