#include "OgreEngineManager.h"

#include <iostream>
#include <sstream>

OgreEngineManager::OgreEngineManager()
  : mRoot(0),
    mGenericCamera(0),
    mWindow(0),
    mIsEngineRunning(true)
{ }

////////////////////////////////////////////////////////////////////////////////

OgreEngineManager::~OgreEngineManager()
{ }

////////////////////////////////////////////////////////////////////////////////

bool OgreEngineManager::Startup(const std::string& pluginFileName,
                                const std::string& configFileName,
                                const std::string& logFileName)
{
  srand((unsigned)time(0));

  mRoot.reset(new Root(pluginFileName, configFileName, logFileName));

  if (!Configure()) {
    return false;
  }

  TextureManager::getSingleton().setDefaultNumMipmaps(5);

  SetupInputDevices();
  SetupResources();

  mRoot->addFrameListener(this);
  WindowEventUtilities::addWindowEventListener(mWindow, this);

  return true;
}

////////////////////////////////////////////////////////////////////////////////

void OgreEngineManager::Shutdown() {
  mRoot->removeFrameListener(this);
  WindowEventUtilities::removeWindowEventListener(mWindow, this);

  mRoot.reset();
  mGenericCamera = 0;
  mWindow = 0;
}

////////////////////////////////////////////////////////////////////////////////

void OgreEngineManager::StartRenderLoop() {
  if (mRoot.get()) {
    mRoot->startRendering();
  }
}

////////////////////////////////////////////////////////////////////////////////

void OgreEngineManager::SetupInputDevices() {
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(make_pair("WINDOW", windowHndStr.str()));

  mInputManager = OIS::InputManager::createInputSystem(pl);

  if (mInputManager->numKeyboards() > 0) {
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->
                                            createInputObject(OIS::OISKeyboard,
                                                              false));
  }

  if (mInputManager->numMice() > 0) {
    mMouse = static_cast<OIS::Mouse*>(mInputManager->
                                      createInputObject(OIS::OISMouse,
                                                        false));
  }
}

////////////////////////////////////////////////////////////////////////////////

void OgreEngineManager::ShutdownInput() {
  if (mInputManager) {
    if (mMouse) mInputManager->destroyInputObject(mMouse);
    if (mKeyboard) mInputManager->destroyInputObject(mKeyboard);

    OIS::InputManager::destroyInputSystem(mInputManager);
    mInputManager = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////

void OgreEngineManager::SetupResources() { }

////////////////////////////////////////////////////////////////////////////////

bool OgreEngineManager::Configure() {
  if (mRoot->showConfigDialog()) {
    mWindow = mRoot->initialise(true, "OGRE");
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////

void OgreEngineManager::windowClosed(RenderWindow* rw) {
  ShutdownInput();
  this->StopRenderLoop();
}

////////////////////////////////////////////////////////////////////////////////

bool OgreEngineManager::frameStarted(const FrameEvent& evt) {
  if (mKeyboard) {
    mKeyboard->capture();

    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
      this->StopRenderLoop();
    }
  }

  if (mMouse) {
    mMouse->capture();
  }

  return mIsEngineRunning;
}
