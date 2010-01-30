#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "gamestate.h"

class menuState : public gameState {
	public:
		void Init();
		void Clean();

		void Suspend();
		void Resume();

		void HandleEvent(Game* game);
		void Update(Game* game);
		void Draw(Game* game);

		static menuState* Instance() {
			return &MenuStateInst;
		}

	protected:
		menuState() {}
	
	private:
		static MenuStateInst;
};
