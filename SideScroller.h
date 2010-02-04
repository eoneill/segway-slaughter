#ifndef SIDESCROLLER_H
#define SIDESCROLLER_H

#include "Actor.h"
#include "gamestate.h"
//#include "Application.h"
#include "Locator.h"

using namespace std;
using namespace Ogre;

class SideScroller : public gameState {
public:
  bool update();

  void initialize(Root* root);
  void clean();

  void suspend();
  void resume();

  //  Singlton instance
  static SideScroller* instance() {
    return &SideScroller_;
  }

protected:
  SideScroller() {}

private:
  static SideScroller SideScroller_;
  Root* root_;
  Actor player;
};
#endif
