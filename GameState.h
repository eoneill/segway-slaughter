////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-02-04 00:51:17 by Eric Scrivner>
//
// Description:
//   Game state class interface
////////////////////////////////////////////////////////////////////////////////
#ifndef GAMESTATE_H_
#define GAMESTATE_H_

// Ogre includes
#include <Ogre.h>

// Other includes
#include "Application.h"

////////////////////////////////////////////////////////////////////////////////
// Class: GameState
//
// Represents a state of the game and handles transitions to other states
class GameState {
public:
  GameState(Ogre::Root* root) 
  { }

  virtual ~GameState()
  { }

  //////////////////////////////////////////////////////////////////////////////
  // Function: update
  //
  // Updates the current game state and returns the next state to go to.
  //
  // Returns NULL if the game should remain in the current state or a valid
  // pointer to the next state if the game should transition states.
  virtual GameState* update() = 0;
};

#endif // GAME_STATE_H_
