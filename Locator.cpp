////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Implements to locator pattern for providing a central place for resource
// access.
////////////////////////////////////////////////////////////////////////////////
#include "Locator.h"

Ogre::Root*    Locator::root_ = 0;
InputSystem*   Locator::inputSystem_ = 0;
CEGUI::System* Locator::ceguiSystem_ = 0;
