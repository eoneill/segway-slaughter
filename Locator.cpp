////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-02-05 16:33:05 by Eric Scrivner>
//
// Description:
//   Implements to locator pattern for providing a central place for resource
// access.
////////////////////////////////////////////////////////////////////////////////
#include "Locator.h"

InputSystem* Locator::inputSystem_ = 0;
Ogre::Root*  Locator::root_ = 0;
