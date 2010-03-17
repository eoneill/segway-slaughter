#ifndef SIDESCROLLER_H
#define SIDESCROLLER_H

#include "Actor.h"
#include "Player.h"
//#include "Enemy.h"
#include "GameState.h"
#include "Locator.h"
#include "AIManager.h"
#include "audio/audio.h"
#include "HUD.h"
//extern const int LEVEL_WIDTH;
const int TIME_PER_LEVEL = (60 * 3) + 30; // 5 minutes = 60 * 5 seconds


class SideScroller : public GameState {
public:
  SideScroller();
  ~SideScroller();

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
  float deathTimer_;
};

#endif
