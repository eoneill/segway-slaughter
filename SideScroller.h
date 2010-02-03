#ifndef SIDESCROLLER_H
#define SIDESCROLLER_H

#include "Actor.h"
#include "gamestate.h"
#include "SegwaySlaughter.h"
#include "Application.h"
#include "Locator.h"

using namespace std;
using namespace Ogre;

class SideScroller : public FrameListener, public gameState {
public:
//  SideScroller(Root* root)
//    : root_(root)
//  { }

  bool frameStarted(const FrameEvent& ev);

  void initialize(Root* root);
//  void clean();

//  void suspend();
//  void resume();
//

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
