#ifndef ENEMY_H
#define ENEMY_H

#include <string>
using namespace std;

const int STATE_DEAD = 0;
const int STATE_IDLE = 1;

class Enemy: public Actor {
	public:
		int state;
};
#endif
