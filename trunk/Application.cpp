////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-01-30 16:44:47 by Eric Scrivner>
//
// Description:
//   Base class for all Ogre applications.
////////////////////////////////////////////////////////////////////////////////
#include "Application.h"
#include "GameState.h"
#include "InputSystem.h"
#include "Locator.h"

#include <iostream>
using namespace std;

Application::Application(const std::string& appName)
  : root_(new Ogre::Root()),
    inputSystem_(0)
{
  defineResources();
  setupRenderSystem();
  createRenderWindow(appName);
  initializeResourceGroups();
  setupScene();
  setupInputSystem();
  createFrameListener();
}

////////////////////////////////////////////////////////////////////////////////

Application::~Application() {
  while(!states_.empty()) {
    states_.back()->clean();
    states_.pop_back();
  }
  delete inputSystem_;
  delete root_;
}

////////////////////////////////////////////////////////////////////////////////

bool Application::frameStarted(const Ogre::FrameEvent& ev) {
  if(states_.empty()) {
    return false;
  }
  inputSystem_->update();
  return states_.back()->update(this);
}

////////////////////////////////////////////////////////////////////////////////

bool Application::update() {
  return true;
}

////////////////////////////////////////////////////////////////////////////////

void Application::go() {
  startRenderLoop();
}

////////////////////////////////////////////////////////////////////////////////

void Application::changeState(GameState* state) {
  if(!states_.empty()) {
    states_.back()->clean();
    states_.pop_back();
  }
  
  states_.push_back(state);
  states_.back()->initialize(root_);
}

/////////////////////////////////////////////////////////////////////////////////

void Application::pushState(GameState* state) {
  if(!states_.empty()) {
    states_.back()->suspend();
  }

  states_.push_back(state);
  states_.back()->initialize(root_);
}

/////////////////////////////////////////////////////////////////////////////////

void Application::popState() {
  if(!states_.empty()) {
    states_.back()->clean();
    states_.pop_back();
  }

  if(!states_.empty()) {
    states_.back()->resume();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Application::setupRenderSystem() {
  if (!root_->restoreConfig() && !root_->showConfigDialog()) {
    throw Ogre::Exception(52,
                          "User cancelled config dialog!",
                          "Application::setupRenderSystem()");
  }
}

////////////////////////////////////////////////////////////////////////////////

void Application::defineResources() {
  Ogre::String secName, typeName, archName;
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");
  
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
  while (seci.hasMoreElements())
  {
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i)
    {
      typeName = i->first;
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName,
                                                                     typeName,
                                                                     secName);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Application::createRenderWindow(const std::string& appName) {
  root_->initialise(true, appName);
}

////////////////////////////////////////////////////////////////////////////////

void Application::initializeResourceGroups() {
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

////////////////////////////////////////////////////////////////////////////////

void Application::setupScene() {
  root_->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
}

////////////////////////////////////////////////////////////////////////////////

void Application::setupInputSystem() {
  inputSystem_ = new InputSystem(root_);
  Locator::registerInput(inputSystem_);
}

////////////////////////////////////////////////////////////////////////////////

void Application::createFrameListener() {
  root_->addFrameListener(this);
}

////////////////////////////////////////////////////////////////////////////////

void Application::startRenderLoop() {
  root_->startRendering();
}
