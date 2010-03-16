#ifndef PARADISE_H
#define PARADISE_H

#include "Actor.h"
#include "Player.h"
//#include "Enemy.h"
#include "GameState.h"
#include "Locator.h"
#include "AIManager.h"
#include "audio/audio.h"
#include "HUD.h"
//extern const int LEVEL_WIDTH;
//const int TIME_PER_LEVEL = (60 * 5); // 5 minutes = 60 * 5 seconds


class Paradise : public GameState {
public:
  Paradise();
  ~Paradise();

  GameState* update(const Ogre::Real& timeSinceLastFrame);
  void initialize();
  bool isDone();
private:
  Actor* player;
  HUD*   hud_;
  bool   isDone_;
  int timeLeft_;
  Ogre::Timer timer_;
  int NumEnemies_;
  audBackground *streetMusic_;
  audSFX *streetSFX_;
  bool bossFight;
};

#endif
