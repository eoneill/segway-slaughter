#ifndef SIDESCROLLER_H
#define SIDESCROLLER_H

#include "Actor.h"
#include "Player.h"
//#include "Enemy.h"
#include "GameState.h"
#include "Locator.h"

extern const int LEVEL_WIDTH;

class SideScroller : public GameState {
public:
  SideScroller();
  ~SideScroller();

  GameState* update();
  void initialize();
  bool isDone();
private:
  Actor player;
  bool isDone_;
};

#endif
