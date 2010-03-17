#include "AIManager.h"
#include <Ogre.h>
#include <iostream>

using namespace std;
using namespace Ogre;

const double ACTIVE_DIST = 1000;
double AICount = 0;
double AITick  = 5000;

double getDist(double pos1[3], double pos2[3]){
	return sqrt(pow((pos1[0] - pos2[0]), 2) + pow((pos1[2] - pos2[2]), 2));
}

void AIManager(vector<Actor*>& actors){
  int minDist = 20;
  int minDistStratH = 100;
  int maxDistStratH = 500;

	//cycle throught all enemies (skip player)
	for(unsigned int i = 1; i < actors.size(); i++)
	{
		//only if not dead
		if(actors[i]->getState() != dead)
    {
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
		    
		    if ((actors[i]->getPosition()[0] - actors[0]->getPosition()[0]) > minDist) {
			  	actors[i]->move(kUp, actors);
			  }
			  if ((actors[i]->getPosition()[0] - actors[0]->getPosition()[0]) < -minDist) {
			  	actors[i]->move(kDown, actors);
			  }
			  if ((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) < -minDist) {
			  	actors[i]->move(kLeft, actors);
			  }
			  if ((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) > minDist) {
			  	actors[i]->move(kRight, actors);
			  }
			  
				actors[i]->attack(actors);
				
				//if it's severely damaged, then start fleeing
				if(actors[i]->getHealth() <= actors[i]->getMaxHealth()/(rand() % 10 + 3) && !actors[i]->isBoss)
				{
					actors[i]->setState(flee);
				}
				else if(rand() % 500 == 30)
			   {
			     actors[i]->setState(strat1);
			   }
				
	    }
	    else if(actors[i]->getState() == strat1)
	    {
	    	// some basic strategy
		    	
		    	if ((actors[i]->getPosition()[0] - actors[0]->getPosition()[0]) > minDist) {
			     actors[i]->move(kDown, actors);
			   }
			   if ((actors[i]->getPosition()[0] - actors[0]->getPosition()[0]) < -minDist) {
			  	  actors[i]->move(kUp, actors);
			   }
			   if ((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) < -minDistStratH) {
			  	  if((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) > -maxDistStratH)
			  	    actors[i]->move(kRight, actors);
			   }
			   if ((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) > minDistStratH) {
			  	  if((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) < maxDistStratH)
                actors[i]->move(kLeft, actors);
			   }
			   
			  
				actors[i]->attack(actors);
				
				//if it's severely damaged, then start fleeing
				
				if(actors[i]->getHealth() <= actors[i]->getMaxHealth()/(rand() % 10 + 3) && !actors[i]->isBoss)
				{
					actors[i]->setState(flee);
				}
				else if(rand() % 300 == 10)
			   {
			     actors[i]->setState(attack);
			   }
	    }
	    else if(actors[i]->getState() == flee)
	    {
	    	//move away from the player
	    	if ((actors[i]->getPosition()[0] - actors[0]->getPosition()[0]) > minDist) {
			  	actors[i]->move(kDown, actors);
			  }
			  if ((actors[i]->getPosition()[0] - actors[0]->getPosition()[0]) < -minDist) {
			  	actors[i]->move(kUp, actors);
			  }
			  if ((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) < -minDist) {
			  	actors[i]->move(kRight, actors);
			  }
			  if ((actors[i]->getPosition()[2] - actors[0]->getPosition()[2]) > minDist) {
			  	actors[i]->move(kLeft, actors);
			  }
			  
			  if(rand() % 1000 == 10)
			   {
			     actors[i]->setState(attack);
			   }
	    }
	  }
	}
}



void spawnBehind(vector<Actor*>& actors, int & NumEnemies, double timeSinceLastFrame)
{

	//only do AI once every tick
	if(AICount <= AITick){
		AICount+= 1000*timeSinceLastFrame;
		return;
	}
	AICount = 0;
	NumEnemies++;
	char EntName[40] = "Mobster";
  sprintf(EntName,"mobster%d",NumEnemies);
//  Ogre::Vector3 pos = actors[0]->getPosition()[2];
  Actor* temp = new Actor(EntName,"mobster_fullanim.mesh", Status(25),
  	                      Ogre::Vector3(rand() % LEVEL_WIDTH - LEVEL_WIDTH/2,0, actors[0]->getPosition()[2] + 1200));
  SceneNode * tempSceneNode = temp->getSceneNode();
	tempSceneNode->yaw(Ogre::Degree(180));
	temp->setDamage(0.01);
	temp->setSpeed(1.2);
	temp->setAttackBox(75);
	temp->setState(attack);
	temp->isEnemy = true;
	temp->setupAnimation();
	
	
  actors.push_back(temp);
}
