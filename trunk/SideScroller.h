#ifndef SIDESCROLLER_H
#define SIDESCROLLER_H

#include "Actor.h"
#include "Player.h"
//#include "Enemy.h"
#include "GameState.h"
#include "Locator.h"
#include "AIManager.h"

//extern const int LEVEL_WIDTH;

class SideScroller : public GameState {
public:
  SideScroller();
  ~SideScroller();

  GameState* update(const Ogre::Real& timeSinceLastFrame);
  void initialize();
  bool isDone();
private:
  Actor* player;
  bool isDone_;
  vector <Actor*> actors;
};

#endif
