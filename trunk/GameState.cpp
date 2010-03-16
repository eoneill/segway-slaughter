#include "GameState.h"

GameState::~GameState() {
  /*for (std::vector<Actor*>::iterator it = actors.begin();
       it != actors.end();) {
    delete *it;
    it = actors.erase(it);
  }

  for (std::vector<Item*>::iterator it = items.begin();
       it != items.end();) {
    delete *it;
    it = items.erase(it);
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

////////////////////////////////////////////////////////////////////////////////
