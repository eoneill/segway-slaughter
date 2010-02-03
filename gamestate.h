#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Application.h"
#include "SegwaySlaughter.h"

using namespace std;
using namespace Ogre;

class gameState {
     public:
          virtual void initialize(Root* root) = 0;
//          virtual void clean() = 0;

//          virtual void suspend() = 0;
//          virtual void resume() = 0;

//          virtual void handleEvent(Game* game) = 0;
//          virtual void update(Game* game) = 0;
//          virtual void draw(Game* game) = 0;

//          void changeState(SegwaySlaughter* game, gameState* state) {
//               game->changeState(state);
//          }
     protected:
          gameState() {}
};

#endif
