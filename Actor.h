#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include <iostream>
#include <Ogre.h>

using namespace std;
using namespace Ogre;

const int DI_UP = 0;
const int DI_DOWN = 1;
const int DI_LEFT = 2;
const int DI_RIGHT = 3;

class Actor {
	public:
	  //string model_name;
	  string sceneNode;
		double position[3];
		float speed;
		bool facingRight;
		float MaxHealth;
		float CurrentHealth;
		float CollisionSideLength;
		//float attackTime;
		//float maxAttackTime;
		bool attacking;
		float damage;
		virtual ~Actor(){};
		//change this to take in the player and enemies by reference later
		virtual bool move(int direction, vector<Actor*> actors){
			//find out where we would move if we moved
			double TempPos[3];
			TempPos[0] = position[0];
			TempPos[1] = position[1];
			TempPos[2] = position[2];
			if(direction == DI_UP){
				TempPos[0] -= speed;
			} else if(direction == DI_DOWN){
				TempPos[0] += speed;
			} else if(direction == DI_LEFT){
				TempPos[2] += speed;
			} else if(direction == DI_RIGHT){
				TempPos[2] -= speed;
			}
			
			//check if it's a valid move
			bool valid = true;
			//x and z value for each point of the hit box
			float this_box[4][2];
			this_box[0][0] = TempPos[0] - CollisionSideLength/2;
			this_box[0][1] = TempPos[2] + CollisionSideLength/2;

			this_box[1][0] = TempPos[0] - CollisionSideLength/2;
			this_box[1][1] = TempPos[2] - CollisionSideLength/2;
			
			this_box[2][0] = TempPos[0] + CollisionSideLength/2;
			this_box[2][1] = TempPos[2] + CollisionSideLength/2;
			
			this_box[3][0] = TempPos[0] + CollisionSideLength/2;
			this_box[3][1] = TempPos[2] - CollisionSideLength/2;
			
			for(unsigned int i = 0; i < actors.size(); i++)
			{
				//get actor's hit box
				float actor_box[4][2];
				actor_box[0][0] = actors[i]->position[0] - actors[i]->CollisionSideLength/2;
				actor_box[0][1] = actors[i]->position[2] + actors[i]->CollisionSideLength/2;

				actor_box[1][0] = actors[i]->position[0] - actors[i]->CollisionSideLength/2;
				actor_box[1][1] = actors[i]->position[2] - actors[i]->CollisionSideLength/2;
				
				actor_box[2][0] = actors[i]->position[0] + actors[i]->CollisionSideLength/2;
				actor_box[2][1] = actors[i]->position[2] + actors[i]->CollisionSideLength/2;
				
				actor_box[3][0] = actors[i]->position[0] + actors[i]->CollisionSideLength/2;
				actor_box[3][1] = actors[i]->position[2] - actors[i]->CollisionSideLength/2;
				
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
			}
			return valid;
		}
		
		void attack(vector<Actor*> actors, Ogre::Root* root_, float damage){
			//find out where the damage box is, based on direction facing
			double damagePos[3];
			damagePos[0] = position[0];
			damagePos[1] = position[1];
			damagePos[2] = position[2] + CollisionSideLength - (facingRight*2*CollisionSideLength);
			
			//x and z value for each point of the damage box
			float damage_box[4][2];
			damage_box[0][0] = damagePos[0] - CollisionSideLength/2;
			damage_box[0][1] = damagePos[2] + CollisionSideLength/2;

			damage_box[1][0] = damagePos[0] - CollisionSideLength/2;
			damage_box[1][1] = damagePos[2] - CollisionSideLength/2;
			
			damage_box[2][0] = damagePos[0] + CollisionSideLength/2;
			damage_box[2][1] = damagePos[2] + CollisionSideLength/2;
			
			damage_box[3][0] = damagePos[0] + CollisionSideLength/2;
			damage_box[3][1] = damagePos[2] - CollisionSideLength/2;
			
			for(unsigned int i = 0; i < actors.size(); i++)
			{
				//get actor's hit box
				float actor_box[4][2];
				actor_box[0][0] = actors[i]->position[0] - actors[i]->CollisionSideLength/2;
				actor_box[0][1] = actors[i]->position[2] + actors[i]->CollisionSideLength/2;

				actor_box[1][0] = actors[i]->position[0] - actors[i]->CollisionSideLength/2;
				actor_box[1][1] = actors[i]->position[2] - actors[i]->CollisionSideLength/2;
				
				actor_box[2][0] = actors[i]->position[0] + actors[i]->CollisionSideLength/2;
				actor_box[2][1] = actors[i]->position[2] + actors[i]->CollisionSideLength/2;
				
				actor_box[3][0] = actors[i]->position[0] + actors[i]->CollisionSideLength/2;
				actor_box[3][1] = actors[i]->position[2] - actors[i]->CollisionSideLength/2;
				
				for(int j = 0; j < 4; j++)					
				{
					if(damage_box[j][1] <= actor_box[0][1] &&
						 damage_box[j][1] >= actor_box[1][1] &&
						 damage_box[j][0] >= actor_box[0][0] &&
						 damage_box[j][0] <= actor_box[2][0]
						 )
					{
						//if we get here, the actor is hit by the attack
						actors[i]->onDamage(damage, root_);
					}
				}
			}
		}
		
		//this might be a virtual function later
		void onDamage(float damage, Ogre::Root* root_){
			CurrentHealth -= damage;
			if(CurrentHealth <= 0){
				onDeath(root_);
			}
		}
		
		//this will be a virtual function later
		void onDeath(Ogre::Root* root_)
		{
			//in leiu of actually destroying it, right now, we'll just move it.
			position[1]+= 10000;
			position[2]+= 10000;
			//Ogre::Root* root_ = getRoot();
			SceneNode* node = root_->getSceneManager("Default SceneManager")->getSceneNode(sceneNode);
			node->translate(Vector3(0,10000,10000));
		
		}
};


#endif
