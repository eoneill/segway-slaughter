#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "game.h"

class gameState {
	public:
		virtual void Init() = 0;
		virtual void Cleanup() = 0;

		virtual void Suspend() = 0;
		virtual void Resume() = 0;

		virtual void HandleEvent(Game* game) = 0;
		virtual void Update(Game* game) = 0;
		virtual void Draw(Game* game) = 0;

		void ChangeState(Game* game, gameState* state) {
			game ->ChangeState(state);
		}
	private:
		gameState() {}
};

#endif
