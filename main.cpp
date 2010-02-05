#include <Ogre.h>
#include "Application.h"
#include "Locator.h"
#include "SideScroller.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace Ogre;

//hopefully replace this with something in the level file itself, later
//assuming it's not a standard size
const int LEVEL_WIDTH = 1000;

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
    app.changeState(SideScroller::instance());
    app.go();
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
