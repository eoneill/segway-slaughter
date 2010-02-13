////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-02-12 15:53:01 by Eric Scrivner>
//
// Description:
//   A simple wrapper class for the OIS input system
////////////////////////////////////////////////////////////////////////////////
#include "InputSystem.h"

#include <algorithm>
#include <functional>

#include <iostream>
using namespace std;
using namespace Ogre;

InputSystem::InputSystem(Ogre::Root* root)
  : inputManager_(createInputManager(root)),
    keyboard_(createKeyboard()),
    mouse_(createMouse())
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

bool InputSystem::isKeyDown(OIS::KeyCode key) {
	assert(keyboard_);
  return keyboard_->isKeyDown(key);
}

////////////////////////////////////////////////////////////////////////////////

void InputSystem::addKeyListener(OIS::KeyListener* listener) {
	if (keyboard_) {
		// Avoid adding duplicates
		if (std::find(keyListeners_.begin(),
		              keyListeners_.end(),
		              listener) == keyListeners_.end()) {
			keyListeners_.push_back(listener);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void InputSystem::removeKeyListener(OIS::KeyListener* listener) {
  if (keyboard_) {
    std::vector<OIS::KeyListener*>::iterator it;
    it = std::find(keyListeners_.begin(),
                   keyListeners_.end(),
                   listener);
    if (it != keyListeners_.end()) {
      keyListeners_.erase(it);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void InputSystem::addMouseListener(OIS::MouseListener* listener) {
	if (mouse_) {
		if (std::find(mouseListeners_.begin(),
		              mouseListeners_.end(),
		              listener) == mouseListeners_.end()) {
			mouseListeners_.push_back(listener);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void InputSystem::removeMouseListener(OIS::MouseListener* listener) {
  if (mouse_) {
    std::vector<OIS::MouseListener*>::iterator it;
    it = std::find(mouseListeners_.begin(),
                   mouseListeners_.end(),
                   listener);
    if (it != mouseListeners_.end()) {
      mouseListeners_.erase(it);
    }
  }
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

OIS::Keyboard* InputSystem::createKeyboard() {
	OIS::Keyboard* keyboard = static_cast<OIS::Keyboard*>(inputManager_->
	                                                      createInputObject(
																														 OIS::OISKeyboard,
																														 true));
	keyboard->setEventCallback(this);

	return keyboard;
}

////////////////////////////////////////////////////////////////////////////////

OIS::Mouse* InputSystem::createMouse() {
	OIS::Mouse* mouse = static_cast<OIS::Mouse*>(inputManager_->
	                                             createInputObject(OIS::OISMouse,
	                                                               true));
	mouse->setEventCallback(this);

	return mouse;
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

////////////////////////////////////////////////////////////////////////////////

bool InputSystem::keyPressed(const OIS::KeyEvent& event) {
	for (std::vector<OIS::KeyListener*>::iterator it = keyListeners_.begin();
			 it != keyListeners_.end(); it++) {
		// Any method which returns false halts event propogation
		if (!(*it)->keyPressed(event)) {
			break;
		}
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool InputSystem::keyReleased(const OIS::KeyEvent& event) {
	for (std::vector<OIS::KeyListener*>::iterator it = keyListeners_.begin();
			 it != keyListeners_.end(); it++) {
		if (!(*it)->keyReleased(event)) {
			break;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool InputSystem::mouseMoved(const OIS::MouseEvent& event) {
  for (std::vector<OIS::MouseListener*>::iterator it = mouseListeners_.begin();
       it != mouseListeners_.end(); it++) {
    if (!(*it)->mouseMoved(event)) {
      break;
    }
  }

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool InputSystem::mousePressed(const OIS::MouseEvent& event,
                               const OIS::MouseButtonID id) {
  for (std::vector<OIS::MouseListener*>::iterator it = mouseListeners_.begin();
       it != mouseListeners_.end(); it++) {
    if (!(*it)->mousePressed(event, id)) {
      break;
    }
  }

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool InputSystem::mouseReleased(const OIS::MouseEvent& event,
                                const OIS::MouseButtonID id) {
  for (std::vector<OIS::MouseListener*>::iterator it = mouseListeners_.begin();
       it != mouseListeners_.end(); it++) {
    if (!(*it)->mouseReleased(event, id)) {
      break;
    }
  }
  
	return true;
}
