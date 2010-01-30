Game::Game() {
}

Game::Init() {
}

Game::ChangeState(gameState* state) {
	if(!states.empty()) {
		states.back()->Clean();
		states.pop_back();
	}

	states.push_back(state);
	states.back()->Init();
}

Game::PushState(gameState* state) {
	if(!states.empty()) {
		states.back()->Suspend();
	}

	states.push_back(state);
	states.back()->Init();
}

Game::PopState() {
	if(!states.empty()) {
		states.back()->Clean();
		states.pop_back();
	}

	if(!states.empty()) {
		states.back()->Resume();
	}
}

Game::HandleEvents() {
	states.back()->HandleEvents(this);
}

Game::Draw() {
	states.back()->Draw(this);
}

Game::Clean() {
	while(!states.empty()) {
		states.back()->Clean();
		states.pop_back();
	}
}
