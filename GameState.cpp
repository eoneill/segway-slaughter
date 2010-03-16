#include "GameState.h"
#include <iostream>
#include <iomanip>
using namespace std;

GameState::~GameState() {
  /*Actor* actor_ptr;
  while (!actors.empty()) {
    actor_ptr = actors.front();
    actors.erase(actors.begin());
    delete actor_ptr;
  }
  
  Item* item_ptr;
  while (!items.empty()) {
    item_ptr = items.front();
    items.erase(items.begin());
    delete item_ptr;
  }*/
}

////////////////////////////////////////////////////////////////////////////////

void GameState::removeDead() {
	for (unsigned int i = 1; i < actors.size(); i++) {
		if (actors[i]->getState() == dead) {
			delete actors[i];
			actors.erase(actors.begin() + i);
		}
	}
}
