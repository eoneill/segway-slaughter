#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Application.h"

using namespace std;
using namespace Ogre;

class gameState : public FrameListener {
  public:

    /////////////////
    //
    //  Use the initalize function to initialize what the
    //  new state needs (Like cameras and players)
    //
    virtual void initialize(Root* root) = 0;

    ///////////////
    //
    //  Use the clean function to clean out what was initalized
    //  from the initialize function
    //
    virtual void clean() = 0;

    //////////////
    //
    //  Use the suspend function to specify what to do when this
    //  states gets suspended
    //
    virtual void suspend() = 0;

    //////////////
    //
    //  Opposite of suspend
    //
    virtual void resume() = 0;

    //////////////
    //
    //  This gets called when ogre calls frameStarted
    //
    virtual bool update() = 0;

//          Not quite yet
//
//          void changeState(Application* app, gameState* state) {
//               app->changeState(state);
//          }
     protected:
          gameState() {}
};

#endif
