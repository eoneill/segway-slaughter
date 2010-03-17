#include "GameState.h"
#include <iostream>
#include <iomanip>
using namespace std;

GameState::~GameState() {
  while (!actors.empty()) {
    delete actors.back();
    actors.pop_back();
  }

  while (!items.empty()) {
    delete items.back();
    items.pop_back();
  }
}

////////////////////////////////////////////////////////////////////////////////

void GameState::removeDead() {
	for (unsigned int i = 1; i < actors.size(); i++) {
		if (actors[i]->deadTick() >= 5000) { //Delete dead after some time
			delete actors[i];
			actors.erase(actors.begin() + i);
		}
	}
}
