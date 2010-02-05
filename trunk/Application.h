////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-01-30 17:28:55 by Eric Scrivner>
//
// Description:
//   Base class for all Ogre applications.
////////////////////////////////////////////////////////////////////////////////
#ifndef APPLICATION_H_
#define APPLICATION_H_

// Ogre includes
#include <Ogre.h>

// C++ includes
#include <string>
#include <vector>

// Other includes
#include "InputSystem.h"
#include "GameState.h"

using namespace std;

class GameState;

////////////////////////////////////////////////////////////////////////////////
// Class: Application
//
// 
class Application : public Ogre::FrameListener {
public:
  //////////////////////////////////////////////////////////////////////////////
  // Function: Application
  //
  // Initializes the application making the title on the window the appName.
  Application(const std::string& appName);
  ~Application();

  //////////////////////////////////////////////////////////////////////////////
  // Function: frameStarted
  //
  // Updates necessary systems each frame. This method should not normally be
  // overriden, and classes  which override this method should have something
  // like the following at the start of their own frameStarted method:
  //
  // bool DerivedClass:frameStarted(const FrameEvent& ev) {
  //   Application::frameStarted(ev);
  //   ... your code here ...
  // }
  //
  // This method should only return false if an application quit has been
  // triggered, otherwise it should return true.
  bool frameStarted(const Ogre::FrameEvent& ev);

  //////////////////////////////////////////////////////////////////////////////
  // Function: update
  //
  // Called before every frame, this is where your application can perform
  // any pre-frame tasks.
  //
  // Returns true if the application should continue running, false otherwise.
  virtual bool update();

  //////////////////////////////////////////////////////////////////////////////
  // Function: go
  //
  // Runs the application
  void go();

  ///////////////////////////////////////////////////////////////////////////////
  // Function: changeState
  //
  // Destroys the current state (if any) and starts the new state passed to it
  void changeState(GameState* state);

  //////////////////////////////////////////////////////////////////////////////
  //  Function: pushState
  //
  //  Suspends the current state (if any) and starts the new state passed to it
  void pushState(GameState* state);

  ///////////////////////////////////////////////////////////////////////////////
  //  Function: popState
  //
  //  Destroys the current state and resumes the previously suspended state
  void popState();
protected:
  void setupRenderSystem();

  void defineResources();

  void createRenderWindow(const std::string& appName);

  void initializeResourceGroups();

  void setupScene();

  void setupInputSystem();

  void createFrameListener();

  void startRenderLoop();

  Ogre::Root*  root_; // The ogre application root object
  InputSystem* inputSystem_; // The input system for the application
  vector<GameState*> states_;  //States for the application
};

#endif // APPLICATION_H_
