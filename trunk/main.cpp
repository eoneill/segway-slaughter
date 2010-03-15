#include <Ogre.h>
#include "Application.h"
#include "MainMenu.h"
#include "SideScroller.h"
#include "CasinoLevel.h"
#include "Actor.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace Ogre;

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
  int main(int *argc, char **argv)
#endif
{
  try
  {
    Application app("Segway Slaughter", argc, argv);
    app.startState(new MainMenu());
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
