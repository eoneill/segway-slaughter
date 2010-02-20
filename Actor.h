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
extern const int LEVEL_WIDTH;

const int ENEMY_POINTS = 10;

class Actor {
private:
  double position_[3];
  float speed_, MaxHealth_, CurrentHealth_, CollisionSideLength_, damage_;
  bool facingRight_, attacking_;
  int score_;
  Ogre::ParticleSystem *pSystem_;
  Ogre::SceneNode *sNode_;
  Ogre::Entity *ent1_;
  Ogre::Root *root_;
public:
  Actor(Ogre::Root *root,
        std::string entName,
        std::string entMesh,
        double position0 = 0,
        double position1 = 0,
        double position2 = 0,
        float speed = 1.33,
        bool facingRight = true,
        float MaxHealth = 100,
        float CurrentHealth = 100,
        float CollisionSideLength = 50,
        bool attacking = false,
        float damage = 0.33
        ) :
    speed_(speed),
    MaxHealth_(MaxHealth),
    CurrentHealth_(CurrentHealth),
    CollisionSideLength_(CollisionSideLength),
    damage_(damage),
    facingRight_(facingRight),
    attacking_(attacking),
    score_(0),
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
  virtual bool move(int direction, vector<Actor*> actors){
    //find out where we would move if we moved
    double TempPos[3];
    TempPos[0] = position_[0];
    TempPos[1] = position_[1];
    TempPos[2] = position_[2];
    if(direction == DI_UP){
      TempPos[0] -= speed_;
    } else if(direction == DI_DOWN){
      TempPos[0] += speed_;
    } else if(direction == DI_LEFT){
      TempPos[2] += speed_;
    } else if(direction == DI_RIGHT){
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
    }

    if(valid) {
      if(direction == DI_UP){
        if(position_[0] > -LEVEL_WIDTH/2){
          sNode_->translate(Vector3(-speed_,0,0));
          position_[0]-=speed_;
        }
      } else if(direction == DI_DOWN){
        if(position_[0] < LEVEL_WIDTH/2){
          sNode_->translate(Vector3(speed_,0,0));
	  position_[0]+=speed_;
        }
      } else if(direction == DI_LEFT){
        sNode_->translate(Vector3(0,0,speed_));
        position_[2]+=speed_;
        
        if(facingRight_ == true){
          sNode_->yaw(Ogre::Degree(180));
          facingRight_ = false;
        }
      } else if(direction == DI_RIGHT){
        sNode_->translate(Vector3(0,0,-speed_));
        position_[2]-=speed_;
					
        if(facingRight_ != true){
          sNode_->yaw(Ogre::Degree(180));
          facingRight_ = true;
        }
      }

    }

    return valid;
  }
    
  void attack(vector<Actor*> &actors, Ogre::Root* root_, float damage = -1){
    if(damage == -1) {
      damage = damage_;
    }
    //find out where the damage box is, based on direction facing
    double damagePos[3];
    damagePos[0] = position_[0];
    damagePos[1] = position_[1];
    damagePos[2] = position_[2] + CollisionSideLength_ - (facingRight_*2*CollisionSideLength_);
      
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
          if(actors[i]->onDamage(damage, root_)) {
            score_ += ENEMY_POINTS;
            delete actors[i];
            actors.erase (actors.begin()+i);
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
