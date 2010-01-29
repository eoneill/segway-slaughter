#include <Ogre.h>
#include "InputSystem.h"
#include "Locator.h"

using namespace Ogre;

class ExitListener : public FrameListener, public OIS::KeyListener {
public:
  ExitListener()
    : isRunning(true)
  {
    Locator::getInput()->addKeyListener(this);
  }

  bool frameStarted(const FrameEvent& ev) {
    return isRunning;
  }
  
  bool keyPressed(const OIS::KeyEvent& arg) {
    if (arg.key == OIS::KC_ESCAPE) {
      isRunning = false;
    }

    return true;
  }

  bool keyReleased(const OIS::KeyEvent& arg) {
    return true;
  }
private:
  bool isRunning;
};

class Application : public FrameListener {
public:
  Application(const std::string& appName)
    : root_(0),
      inputSystem_(0)
  {
    createRoot();
    setupRenderSystem();
    createRenderWindow(appName);
    setupScene();
    setupInputSystem();
    createFrameListener();
    startRenderLoop();
  }

  ~Application() {
    delete inputSystem_;
    delete root_;
  }

  bool frameStarted(const FrameEvent& ev) {
    inputSystem_->update();
    return true;
  }
private:
  void createRoot() {
    root_ = new Root();
  }

  void setupRenderSystem() {
    if (!root_->restoreConfig() && !root_->showConfigDialog()) {
      throw Exception(52,
                      "User cancelled config dialog!",
                      "Application::setupRenderSystem()");
    }
  }

  void createRenderWindow(const std::string& appName) {
    root_->initialise(true, appName);
  }

  void setupScene() {
    root_->createSceneManager(ST_GENERIC, "Default SceneManager");
  }

  void setupInputSystem() {
    inputSystem_ = new InputSystem(root_);
    Locator::registerInput(inputSystem_);
  }

  void createFrameListener() {
    frameListener_ = new ExitListener();
    root_->addFrameListener(this);
    root_->addFrameListener(frameListener_);
  }

  void startRenderLoop() {
    root_->startRendering();
  }

  Ogre::Root*  root_;
  InputSystem* inputSystem_;
  ExitListener* frameListener_;
};

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    try
    {
      Application app("Segway Slaughter");
    }
    catch(Exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
    }

    return 0;
}
