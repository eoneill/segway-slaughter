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

// Other includes
#include "InputSystem.h"
#include "gamestate.h"

using namespace std;

class gameState;

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
  bool update();

  //////////////////////////////////////////////////////////////////////////////
  // Function: initialize  -  Depreciated (Done by constructor)
  //
  // This method is called after Ogre is completely initialized and before the
  // game starts. This is where you should perform any application specific
  // initialization tasks.
  //
  //  virtual void initialize() = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Function: go
  //
  // Runs the application
  void go();

  ///////////////////////////////////////////////////////////////////////////////
  // Function: changeState
  //
  // Destroys the current state (if any) and starts the new state passed to it
  void changeState(gameState* state);

  //////////////////////////////////////////////////////////////////////////////
  //  Function: pushState
  //
  //  Suspends the current state (if any) and starts the new state passed to it
  void pushState(gameState* state);

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
  vector<gameState*> states_;  //States for the application
};

#endif // APPLICATION_H_
