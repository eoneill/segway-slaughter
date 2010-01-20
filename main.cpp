#include "OgreEngineManager.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
  try {
    if (ENGINEMANAGER.Startup("plugins.cfg", "ogre.cfg", "ogre.log")) {
      ENGINEMANAGER.StartRenderLoop();
    }

    ENGINEMANAGER.Shutdown();
  } catch (OIS::Exception& e) {
    cerr << e.eText << endl;

    return 1;
  }

  return 0;
}
