////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-02-24 19:44:01 by Eric Scrivner>
//
// Description:
//   Base class for all Ogre applications.
////////////////////////////////////////////////////////////////////////////////
#include "Application.h"
#include "Locator.h"
#include <AL/alut.h>

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Constants

const char* DEFAULT_SCENE_MGR = "Default SceneManager";

////////////////////////////////////////////////////////////////////////////////

Application::Application(const std::string& appName, int *argc, char **argv)
  : root_(new Ogre::Root()),
    inputSystem_(0),
    ceguiRenderer_(0),
    ceguiSystem_(0)
{
  // Register the root object
  Locator::registerRoot(root_);

  // Initialize everything
  defineResources();
  setupRenderSystem();
  createRenderWindow(appName);
  initializeResourceGroups();
  setupScene();
  setupInputSystem();
  setupCEGUI();
  createFrameListener();
  root_->getAutoCreatedWindow();
  alutInit(argc, argv);
}

////////////////////////////////////////////////////////////////////////////////

Application::~Application() {
  assert(states_.empty());

  delete inputSystem_;
  delete root_;
}

////////////////////////////////////////////////////////////////////////////////

void Application::startState(GameState* state) {
  pushState(state);
}

////////////////////////////////////////////////////////////////////////////////

bool Application::frameStarted(const Ogre::FrameEvent& ev) {
  assert(!states_.empty());

  // Update the input system
  inputSystem_->update();

  // Update the current state and update the state stack as needed
  GameState* nextState = states_.back()->update(ev.timeSinceLastFrame);

  if (states_.back()->isDone()) {
    popState();
  }

  if (nextState != NULL) {
    pushState(nextState);
  }

  // If there are no states left, then quit the application
  if (states_.empty()) {
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

void Application::go() {
  startRenderLoop();
}

/////////////////////////////////////////////////////////////////////////////////

void Application::pushState(GameState* state) {
  state->initialize();
  states_.push_back(state);
}

/////////////////////////////////////////////////////////////////////////////////

void Application::popState() {
  assert(!states_.empty());
  delete states_.back();
  states_.pop_back();
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
  root_->createSceneManager(Ogre::ST_GENERIC, DEFAULT_SCENE_MGR);
  Locator::registerSceneManager(root_->getSceneManager(DEFAULT_SCENE_MGR));
}

////////////////////////////////////////////////////////////////////////////////

void Application::setupInputSystem() {
  inputSystem_ = new InputSystem(root_);
  Locator::registerInput(inputSystem_);
}

////////////////////////////////////////////////////////////////////////////////

void Application::setupCEGUI() {
  Ogre::SceneManager *mgr = Locator::getSceneManager();
  Ogre::RenderWindow *win = root_->getAutoCreatedWindow();

  // CEGUI setup
  ceguiRenderer_ = new CEGUI::OgreCEGUIRenderer(win, Ogre::RENDER_QUEUE_OVERLAY,
                                                false, 3000, mgr);
  ceguiSystem_ = new CEGUI::System(ceguiRenderer_);

  Locator::registerGuiSystem(ceguiSystem_);
	
  CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
  ceguiSystem_->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
  ceguiSystem_->setDefaultMouseCursor("TaharezLook", "MouseArrow");
  CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseMoveCursor");
  
  //CEGUI
  CEGUI::Window* mEditorGuiSheet = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultGUISheet", "root"); 
  ceguiSystem_->setGUISheet(mEditorGuiSheet);
}

////////////////////////////////////////////////////////////////////////////////

void Application::createFrameListener() {
  root_->addFrameListener(this);
}

////////////////////////////////////////////////////////////////////////////////

void Application::startRenderLoop() {
  root_->startRendering();
}

////////////////////////////////////////////////////////////////////////////////
