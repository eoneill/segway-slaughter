#include "AIManager.h"
#include <Ogre.h>
#include <iostream>

using namespace std;
using namespace Ogre;

const double ACTIVE_DIST = 200;
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
		if(actors[i]->state_ != STATE_DEAD)
    {
			SceneNode* node = actors[i]->sNode_;
	    
	    if(actors[i]->state_ == STATE_IDLE)
	    {
	    	//check to see if the player is close enough
	    	//cerr << "IDLE" << endl;
	    	if(fabs(actors[i]->position_[2] - actors[0]->position_[2]) <= ACTIVE_DIST ){
	    		//if so, change to attack state
	    		actors[i]->state_ = STATE_ATTACK;
	    		//cerr << "SWITCH TO ATTACK" << endl;
	    	}
	    } else if(actors[i]->state_ == STATE_ATTACK){
	    	//move toward the player
	    	//Move enemy up, but with constraints
			  if ((actors[i]->position_[0] - actors[0]->position_[0]) > 3) {
			  	if(actors[i]->move(DI_UP, actors))
			  	{
				    if(actors[i]->position_[0] > -1000/2){//####################LEVEL_WIDTH
				      node->translate(Vector3(-actors[i]->speed_,0,0));
				      actors[i]->position_[0]-=actors[i]->speed_;
				    }
				  }
					node->yaw(Ogre::Degree(90*(DI_UP-actors[i]->direction_)));
			    actors[i]->direction_ = DI_UP;
			  }		  
			  //Move enemy down, but with constraints
			  if ((actors[i]->position_[0] - actors[0]->position_[0]) < -3) {
			  	if(actors[i]->move(DI_DOWN, actors))
			  	{
				    if(actors[i]->position_[0] < 1000/2){//####################LEVEL_WIDTH
				      node->translate(Vector3(actors[i]->speed_,0,0));
				      actors[i]->position_[0]+=actors[i]->speed_;
				    }
				  }
					node->yaw(Ogre::Degree(90*(DI_DOWN-actors[i]->direction_)));
			    actors[i]->direction_ = DI_DOWN;
			  }
			  //move enemy left
			    if ((actors[i]->position_[2] - actors[0]->position_[2]) < -3) {
				    if(actors[i]->move(DI_LEFT, actors))
			    	{
			    	  node->translate(Vector3(0,0,actors[i]->speed_));
				      actors[i]->position_[2]+=actors[i]->speed_;
								
							node->yaw(Ogre::Degree(90*(DI_LEFT-actors[i]->direction_)));
					    actors[i]->direction_ = DI_LEFT;
			      }			    
			    }
			    //move enemy right
			    if ((actors[i]->position_[2] - actors[0]->position_[2]) > 3) {
			    	if(actors[i]->move(DI_RIGHT, actors))
			    	{
			    	  node->translate(Vector3(0,0,-actors[i]->speed_));
				      actors[i]->position_[2]-=actors[i]->speed_;
								
							node->yaw(Ogre::Degree(90*(DI_RIGHT-actors[i]->direction_)));
					    actors[i]->direction_ = DI_RIGHT;
			      }
			    }
	    	
	    	
	    	//if we're close enough, attack
	    	//if(getDist(actors[i]->position, actors[0]->position) <  actors[i]->CollisionSideLength){
	    		actors[i]->attack(actors);
	    		//cerr << "ATTACK!!" << endl;
	    	//}
	    	
	    } else if(actors[i]->state_ == STATE_DEAD){
	    	//do nothing
	    }
	  }
	}
}


