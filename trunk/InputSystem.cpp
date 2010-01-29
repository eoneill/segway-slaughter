////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-01-29 14:28:38 by Eric Scrivner>
//
// Description:
//   A simple wrapper class for the OIS input system
////////////////////////////////////////////////////////////////////////////////
#include "InputSystem.h"

using namespace Ogre;

InputSystem::InputSystem(Ogre::Root* root)
  : inputManager_(createInputManager(root)),
    keyboard_(0),
    mouse_(0)
{ }

////////////////////////////////////////////////////////////////////////////////

InputSystem::~InputSystem() {
  if (keyboard_) {
    inputManager_->destroyInputObject(keyboard_);
  }

  if (mouse_) {
    inputManager_->destroyInputObject(mouse_);
  }

  OIS::InputManager::destroyInputSystem(inputManager_);
}

////////////////////////////////////////////////////////////////////////////////

void InputSystem::addKeyListener(OIS::KeyListener* listener) {
  if (!keyboard_) {
    keyboard_ = static_cast<OIS::Keyboard*>(inputManager_->createInputObject(
                                            OIS::OISKeyboard, true));
  }
    
  keyboard_->setEventCallback(listener);
}

////////////////////////////////////////////////////////////////////////////////

void InputSystem::addMouseListener(OIS::MouseListener* listener) {
  if (!mouse_) {
    mouse_ = static_cast<OIS::Mouse*>(inputManager_->createInputObject(
                                      OIS::OISMouse, true));
  }

  mouse_->setEventCallback(listener);
}

////////////////////////////////////////////////////////////////////////////////

void InputSystem::update() {
  if (keyboard_) {
    keyboard_->capture();
  }

  if (mouse_) {
    mouse_->capture();
  }
}

////////////////////////////////////////////////////////////////////////////////

OIS::InputManager* InputSystem::createInputManager(Ogre::Root* root) {
  // Retrieve the window handle from the render window
  Ogre::RenderWindow* renderWin = root->getAutoCreatedWindow();
    
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;
  renderWin->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
    
  // Construct a new buffered input manager for the given window
  OIS::ParamList params;
  params.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    
  return OIS::InputManager::createInputSystem(params);
}
