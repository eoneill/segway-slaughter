////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-01-29 14:55:32 by Eric Scrivner>
//
// Description:
//   Implements to locator pattern for providing a central place for resource
// access.
////////////////////////////////////////////////////////////////////////////////
#ifndef LOCATOR_H_
#define LOCATOR_H_

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
private:
  static InputSystem* inputSystem_; // The input system instance
};

#endif // LOCATOR_H_
