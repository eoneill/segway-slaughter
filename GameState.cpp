#include "GameState.h"

GameState::~GameState() {
	
}

////////////////////////////////////////////////////////////////////////////////

void GameState::removeDead() {
	for (std::vector<Actor*>::iterator it = actors.begin();
			 it != actors.end(); it++) {
		if ((*it)->getState() == dead) {
			delete *it;
			actors.erase(it);
		} else {
			++it;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
