#ifndef CASINOLEVEL_H
#define CASINOLEVEL_H

#include "Actor.h"
#include "Player.h"
//#include "Enemy.h"
#include "GameState.h"
#include "Locator.h"

using namespace std;
using namespace Ogre;

extern const int LEVEL_WIDTH;

class CasinoLevel : public GameState {
public:
  bool update(Application* app);

  void initialize(Root* root);
  void clean();

  void suspend();
  void resume();

  //  Singlton instance
  static CasinoLevel* instance() {
    return &CasinoLevel_;
  }

protected:
  CasinoLevel() {}

private:
  static CasinoLevel CasinoLevel_;
  Root* root_;
  Actor player;
};
#endif
