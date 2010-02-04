#ifndef SIDE_SCROLLER_H_
#define SIDE_SCROLLER_H_

#include "Actor.h"
#include "GameState.h"

class SideScroller : public GameState {
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
#endif // SIDE_SCROLLER_H_
