#ifndef SEGWAYSLAUGHTER_H
#define SEGWAYSLAUGHTER_H

#include <vector>
#include "Application.h"
#include "SideScroller.h"

using namespace std;

class gameState;

class SegwaySlaughter : public Application {
public:
  SegwaySlaughter()
    : Application::Application("Segway Slaughter")//,
//      sideScroller_(0)
  {}

  ~SegwaySlaughter() {
    clean();
  }

  void initialize();
  void changeState(gameState* state);
//  void pushState(gameState* state);
//  void popState();

//  void handleEvents();

//  void update();

//  void draw();

  void clean();

  bool running() { return isRunning; }
  void quit() { isRunning = false; }

private:
//  SideScroller* sideScroller_;
  vector<gameState*> states;
  bool isRunning;
};
#endif
