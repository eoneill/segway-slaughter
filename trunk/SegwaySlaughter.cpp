#include "SegwaySlaughter.h"

void SegwaySlaughter::initialize() {
//	sideScroller_ = new SideScroller(root_);
//	sideScroller_->initialize();

//	root_->addFrameListener(sideScroller_);
}

void SegwaySlaughter::changeState(gameState* state) {
  if(!states.empty()) {
//    states.back()->clean();
    states.pop_back();
  }

  states.push_back(state);
  states.back()->initialize(root_);

}
#if 0
void SegwaySlaughter::pushState(gameState* state) {
  if(!states.empty()) {
    states.back()->suspend();
  }

  states.push_back(state);
  states.back()->initialize();
}

void SegwaySlaughter::popState() {
  if(!states.empty()) {
    states.back()->clean();
    states.pop_back();
  }

  if(!states.empty()) {
    states.back()->resume();
  }
}

SegwaySlaughter::handleEvents() {
  states.back()->HandleEvents(this);
}

SegwaySlaughter::draw() {
  states.back()->Draw(this);
}
#endif

void SegwaySlaughter::clean() {
  while(!states.empty()) {
//    states.back()->clean();
    states.pop_back();
  }
}
