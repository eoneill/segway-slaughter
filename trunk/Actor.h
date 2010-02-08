#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

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
					//cout << this_box[j][1] << " " << actor_box[0][1] << endl;
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
};
#endif
