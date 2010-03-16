////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-03-15 19:58:06 by Eric Scrivner>
//
// Description:
//   Describes an object for defining and managing the state of the game as well
// as transitions between game states.
////////////////////////////////////////////////////////////////////////////////
#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "Actor.h"
#include "Item.h"
#include "Locator.h"

#include <vector>

#include <Ogre.h>

////////////////////////////////////////////////////////////////////////////////
// Class: GameState
//
// Defines the current state of the game
class GameState {
public:
  GameState()
    : root_(Locator::getRoot())
  { }

  virtual ~GameState();

  //////////////////////////////////////////////////////////////////////////////
  // Function: initialize
  //
  // Initializes the current state
  virtual void initialize() = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Function: update
  //
  // Updates the current state and returns the next state (which may be this
  // state).
  virtual GameState* update(const Ogre::Real& timeSinceLastFrame) = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Function: isDone
  //
  // Returns true if this state is done executing, false otherwise.
  virtual bool isDone() = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Function: suspend
  //
  // Called when a new state is pushed onto the stack and this state was the
  // former top of the stack.
  // virtual void suspend() = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Function: resume
  //
  // Called when a state returns to the top of the stack
  // virtual void resume() = 0;
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: removeDead
  //
  // Cleanup any dead actors
	void removeDead();

  std::vector <Actor*> actors; // All the actors in the application
  std::vector <Item*>  items; // All items in the level
protected:
  //////////////////////////////////////////////////////////////////////////////
  // Function: getRoot
  //
  // Returns the root pointer
  Ogre::Root* getRoot()
  { return root_; }
private:
  Ogre::Root* root_;
};

#endif // GAME_STATE_H_
