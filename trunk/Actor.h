#ifndef ACTOR_H
#define ACTOR_H

#include <string>
using namespace std;


class Actor {
	public:
	  //string model_name;
	  string sceneNode;
		double position[3];
		float speed;
		bool facingRight;
		float MaxHealth;
		float CurrentHealth;
};
#endif
