#ifndef CASINOLEVEL_H
#define CASINOLEVEL_H

#include "Actor.h"
#include "Player.h"
//#include "Enemy.h"
#include "GameState.h"
#include "Locator.h"

//extern const int LEVEL_WIDTH;

class CasinoLevel : public GameState {
public:
  CasinoLevel();
  ~CasinoLevel();

  GameState* update();
  void initialize();
  bool isDone();
private:
  Actor* player;
  bool isDone_;
  vector <Actor*> enemies;
};

#endif
