#ifndef GAME_H
#define GAME_H

#include <vector>

using namespace std;

class gameState;

class Game {
	public:
		Game();

		void Init();
		void ChangeState(gameState* state);
		void PushState(gameState* state);
		void PopState();

		void HandleEvents();

		void Update();

		void Draw();

		void Clean();

		bool Running() { return isRunning; }
		void Quit() { isRunning = false; }

	private:
		vector<gameState*> states;
		bool isRunning;
};
#endif
