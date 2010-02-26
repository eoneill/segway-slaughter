#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include <iostream>
#include <Ogre.h>

using namespace std;
using namespace Ogre;

const int DI_DOWN = 0;
const int DI_RIGHT = 1;
const int DI_UP = 2;
const int DI_LEFT = 3;
extern const int LEVEL_WIDTH;

const int ENEMY_POINTS = 10;

const int STATE_DEAD = 0;
const int STATE_IDLE = 1;
const int STATE_ATTACK = 2;

class Actor {
public:
  double position_[3];
  float speed_, MaxHealth_, CurrentHealth_, CollisionSideLength_, damage_;
  bool attacking_;
  //state should just be in enemy... worry about that later
  int direction_, score_, state_;
  Ogre::ParticleSystem *pSystem_;
  Ogre::SceneNode *sNode_;
  Ogre::Entity *ent1_;
  Ogre::Root *root_;
//public:
  Actor(Ogre::Root *root,
        std::string entName,
        std::string entMesh,
        double position0 = 0,
        double position1 = 0,
        double position2 = 0,
        float speed = 1.33,
        int direction = DI_RIGHT,
        int state = STATE_IDLE,
        float MaxHealth = 100,
        float CurrentHealth = 100,
        float CollisionSideLength = 50,
        bool attacking = false,
        float damage = 1
        ) :
    speed_(speed),
    MaxHealth_(MaxHealth),
    CurrentHealth_(CurrentHealth),
    CollisionSideLength_(CollisionSideLength),
    damage_(damage),
    attacking_(attacking),
    direction_(direction),
    score_(0),
    state_(1),
    root_(root)
    {
      position_[0] = position0;
      position_[1] = position1;
      position_[2] = position2;
      Ogre::SceneManager* mSceneMgr = root_->getSceneManager("Default SceneManager");
      ent1_ = mSceneMgr->createEntity( entName, entMesh);
      sNode_ = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(position0,position1,position2));
      pSystem_ = mSceneMgr->createParticleSystem( entName+" particle","Blood");
      pSystem_->getEmitter(0)->setEmissionRate(0);
      sNode_->attachObject( ent1_ );
      sNode_->attachObject( pSystem_ );
    }

  virtual  ~Actor(){
    Ogre::SceneManager* mSceneMgr = root_->getSceneManager("Default SceneManager");
    sNode_->detachObject(ent1_);
    //mSceneMgr->destroyParticleSystem(pSystem_);
    pSystem_->getEmitter(0)->setEmissionRate(0);
    //mSceneMgr->getRootSceneNode()->removeAndDestroyChild( entName + " particle");
    mSceneMgr->destroyEntity(ent1_);
  };
    
  //change this to take in the player and enemies by reference later
  virtual bool move(int newDirection, vector<Actor*> actors){
    //find out where we would move if we moved
    double TempPos[3];
    TempPos[0] = position_[0];
    TempPos[1] = position_[1];
    TempPos[2] = position_[2];
    if(newDirection == DI_UP){
      TempPos[0] -= speed_;
    } else if(newDirection == DI_DOWN){
      TempPos[0] += speed_;
    } else if(newDirection == DI_LEFT){
      TempPos[2] += speed_;
    } else if(newDirection == DI_RIGHT){
      TempPos[2] -= speed_;
    }
    
    //check if it's a valid move
    bool valid = true;
    //x and z value for each point of the hit box
    float this_box[4][2];
    this_box[0][0] = TempPos[0] - CollisionSideLength_/2;
    this_box[0][1] = TempPos[2] + CollisionSideLength_/2;
    this_box[1][0] = TempPos[0] - CollisionSideLength_/2;
    this_box[1][1] = TempPos[2] - CollisionSideLength_/2;
     
    this_box[2][0] = TempPos[0] + CollisionSideLength_/2;
    this_box[2][1] = TempPos[2] + CollisionSideLength_/2;
    
    this_box[3][0] = TempPos[0] + CollisionSideLength_/2;
    this_box[3][1] = TempPos[2] - CollisionSideLength_/2;
    
    for(unsigned int i = 0; i < actors.size(); i++)
    {
      //a quick check first
			/*if(getDist(TempPos, actors[i]->position) >= 30){
						valid = true;
						return valid;
				}	*/			
				//don't collide with itself, and do a quick check as well
				if(sNode_ != actors[i]->sNode_ &&	fabs(actors[i]->position_[0] - TempPos[0])
				< actors[i]->CollisionSideLength_ + CollisionSideLength_)
				{
					//get actor's hit box
					float actor_box[4][2];
					actor_box[0][0] = actors[i]->position_[0] - actors[i]->CollisionSideLength_/2;
					actor_box[0][1] = actors[i]->position_[2] + actors[i]->CollisionSideLength_/2;

					actor_box[1][0] = actors[i]->position_[0] - actors[i]->CollisionSideLength_/2;
					actor_box[1][1] = actors[i]->position_[2] - actors[i]->CollisionSideLength_/2;
					
					actor_box[2][0] = actors[i]->position_[0] + actors[i]->CollisionSideLength_/2;
					actor_box[2][1] = actors[i]->position_[2] + actors[i]->CollisionSideLength_/2;
					
					actor_box[3][0] = actors[i]->position_[0] + actors[i]->CollisionSideLength_/2;
					actor_box[3][1] = actors[i]->position_[2] - actors[i]->CollisionSideLength_/2;
					
					for(int j = 0; j < 4; j++)					
					{
						if(this_box[j][1] <= actor_box[0][1] &&
							 this_box[j][1] >= actor_box[1][1] &&
							 this_box[j][0] >= actor_box[0][0] &&
							 this_box[j][0] <= actor_box[2][0]
							 )
							{
								valid = false; 
							}
					}
					/*if(getDist(TempPos, actors[i]->position) <= actors[i]->CollisionSideLength + CollisionSideLength){
						valid = false;
					}*/
				}
    }

    if(newDirection == DI_UP){
   		if(valid)
      {
	     	if(position_[0] > -LEVEL_WIDTH/2){
	  	     sNode_->translate(Vector3(-speed_,0,0));
  	      position_[0]-=speed_;
  	    }
  		}
    }
    if(newDirection == DI_DOWN){
      if(valid)
  	  {
	      if(position_[0] < LEVEL_WIDTH/2){
	        sNode_->translate(Vector3(speed_,0,0));
	        position_[0]+=speed_;
	      }
	    }
    }
    if(newDirection == DI_LEFT){
      if(valid)
    	{
	      sNode_->translate(Vector3(0,0,speed_));
	      position_[2]+=speed_;
	    }
    }
    if(newDirection == DI_RIGHT){
      if(valid)
    	{
	      sNode_->translate(Vector3(0,0,-speed_));
	      position_[2]-=speed_;
      }
    }

		//rotate regardless of collision
		sNode_->yaw(Ogre::Degree(90*(newDirection-direction_)));
    direction_ = newDirection;

    return valid;
  }
    
  void attack(vector<Actor*> &actors, Ogre::Root* root_){
    //find out where the damage box is, based on direction facing
			double damagePos[3];
			int vert = 0;
			int horiz = 0;
			if(direction_ == DI_UP){
				vert = -1;
			}else if(direction_ == DI_DOWN){
				vert = 1;
			}else if(direction_ == DI_LEFT){
				horiz = 1;
			}else if(direction_ == DI_RIGHT){
				horiz = -1;
			}
			
			damagePos[0] = position_[0] + ( 2*CollisionSideLength_*vert );
			damagePos[1] = position_[1];
			damagePos[2] = position_[2] + ( 2*CollisionSideLength_*horiz );
			
			//x and z value for each point of the damage box
			float damage_box[4][2];
			damage_box[0][0] = damagePos[0] - CollisionSideLength_/2;
			damage_box[0][1] = damagePos[2] + CollisionSideLength_/2;

			damage_box[1][0] = damagePos[0] - CollisionSideLength_/2;
			damage_box[1][1] = damagePos[2] - CollisionSideLength_/2;
			
			damage_box[2][0] = damagePos[0] + CollisionSideLength_/2;
			damage_box[2][1] = damagePos[2] + CollisionSideLength_/2;
			
			damage_box[3][0] = damagePos[0] + CollisionSideLength_/2;
			damage_box[3][1] = damagePos[2] - CollisionSideLength_/2;
			
			for(unsigned int i = 0; i < actors.size(); i++)
			{
				//don't collide with itself and do a quick check
				if(sNode_ != actors[i]->sNode_ && fabs(actors[i]->position_[0] - position_[0])
				< actors[i]->CollisionSideLength_ + CollisionSideLength_)
				{
				//get actor's hit box
					float actor_box[4][2];
					actor_box[0][0] = actors[i]->position_[0] - actors[i]->CollisionSideLength_/2;
					actor_box[0][1] = actors[i]->position_[2] + actors[i]->CollisionSideLength_/2;

					actor_box[1][0] = actors[i]->position_[0] - actors[i]->CollisionSideLength_/2;
					actor_box[1][1] = actors[i]->position_[2] - actors[i]->CollisionSideLength_/2;
					
					actor_box[2][0] = actors[i]->position_[0] + actors[i]->CollisionSideLength_/2;
					actor_box[2][1] = actors[i]->position_[2] + actors[i]->CollisionSideLength_/2;
					
					actor_box[3][0] = actors[i]->position_[0] + actors[i]->CollisionSideLength_/2;
					actor_box[3][1] = actors[i]->position_[2] - actors[i]->CollisionSideLength_/2;
					
					for(int j = 0; j < 4; j++)					
					{
						if(damage_box[j][1] <= actor_box[0][1] &&
							 damage_box[j][1] >= actor_box[1][1] &&
							 damage_box[j][0] >= actor_box[0][0] &&
							 damage_box[j][0] <= actor_box[2][0]
							 )
						{
							//if we get here, the actor is hit by the attack
						if(actors[i]->onDamage(damage_, root_)) {
  	          score_ += ENEMY_POINTS;
    	        delete actors[i];
      	      actors.erase (actors.begin()+i);
        	  }
					}
				}
			}
	  }
  }





  void stopBlood() {
    pSystem_->getEmitter(0)->setEmissionRate(0);
  }
    
  //this might be a virtual function later
  bool onDamage(float damage, Ogre::Root* root_){
    pSystem_->getEmitter(0)->setEmissionRate(100);
    CurrentHealth_ -= damage;
    if(CurrentHealth_ <= 0){
      onDeath(root_);
      return true;
    }
    return false;
  }
    
  //this will be a virtual function later
  void onDeath(Ogre::Root* root_)
  {
    //in leiu of actually destroying it, right now, we'll just move it.
    position_[1]+= 10000;
    position_[2]+= 10000;
    sNode_->translate(Vector3(0,10000,10000));
    state_ = STATE_DEAD;
  }

  int getScore() const {
    return score_;
  }
  
  float getHealth() const {
    return CurrentHealth_;
  }

  float maxHealth() const {
    return  MaxHealth_;
  }
};


#endif
