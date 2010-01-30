////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-01-29 17:09:34 by Eric Scrivner>
//
// Description:
//   Base class for all Ogre applications.
////////////////////////////////////////////////////////////////////////////////
#include "Application.h"
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
  delete inputSystem_;
  delete root_;
}

////////////////////////////////////////////////////////////////////////////////

bool Application::frameStarted(const Ogre::FrameEvent& ev) {
  inputSystem_->update();
  return true;
}

////////////////////////////////////////////////////////////////////////////////

void Application::go() {
  initialize();
  startRenderLoop();
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
