#include "AIManager.h"
#include <Ogre.h>
#include <iostream>

using namespace std;
using namespace Ogre;

const double ACTIVE_DIST = 1000;
int AICount = 0;
int AITick  = 0;


double getDist(double pos1[3], double pos2[3]){
	return sqrt(pow((pos1[0] - pos2[0]), 2) + pow((pos1[2] - pos2[2]), 2));
}

void AIManager(vector<Actor*> actors){
	//only do AI once every tick
	if(AICount != AITick){
		//cout << "AI count" << endl;
		AICount++;
		return;
	}
	AICount = 0;
	//cout << "AI Tick" << endl;

	//cycle throught all enemies (skip player)
	for(unsigned int i = 1; i < actors.size(); i++)
	{
		//only if not dead
		if(actors[i]->getState() != dead)
    {
			//SceneNode * node = actors[i]->getSceneNode();
	    if(actors[i]->getState() == idle)
	    {
				//check to see if the player is close enough
		    if(fabs(actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) <= ACTIVE_DIST )
		    {
	   			//if so, change to attack state
	   			actors[i]->setState(attack);
	   		}
	    } else if(actors[i]->getState() == attack)
	    {
		    //move toward the player
		    if ((actors[i]->getPosition()[0] - actors[0]->getPosition()[0]) > 3) {
			  	actors[i]->move(kUp, actors);
			  }
			  if ((actors[i]->getPosition()[0] - actors[0]->getPosition()[0]) < -3) {
			  	actors[i]->move(kDown, actors);
			  }
			  if ((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) < -3) {
			  	actors[i]->move(kLeft, actors);
			  }
			  if ((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) > 3) {
			  	actors[i]->move(kRight, actors);
			  }
				actors[i]->attack(actors);
	    	
	    }
	  }
	}
}
