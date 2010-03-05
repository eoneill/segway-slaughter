#ifndef CASINOLEVEL_H
#define CASINOLEVEL_H

#include "Actor.h"
#include "Player.h"
//#include "Enemy.h"
#include "GameState.h"
#include "Locator.h"
//#include "AIManager.h"

//extern const int LEVEL_WIDTH;

class CasinoLevel : public GameState {
public:
  CasinoLevel();
  ~CasinoLevel();

  GameState* update(const Ogre::Real& timeSinceLastFrame);
  void initialize();
  bool isDone();
private:
  Actor* player;
  bool isDone_;
  vector <Actor*> actors;
};

#endif
