////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Class which defines a character in the game
////////////////////////////////////////////////////////////////////////////////
#include "Actor.h"
#include "Locator.h"

using namespace std;
using namespace Ogre;

bool CylinderHit(const Ogre::Vector3& pos1,
                 const Ogre::Vector3& pos2) {
  if (sqrt(pow(pos1.x - pos2.x, 2) + 
           pow(pos1.z - pos2.z, 2)) < DEFAULT_COLLISION_RAD) {
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

Actor::Actor(const std::string& entityName,
             const std::string& entityMesh,
             const Status& stats,
             const Ogre::Vector3& position)
  : position_(position),
    direction_(kRight),
    stats_(stats)
{
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  entity_ = sceneMgr->createEntity(entityName, entityMesh);

  sceneNode_ = sceneMgr->getRootSceneNode()->createChildSceneNode(position_);
  sceneNode_->attachObject(entity_);
  //sceneNode_->showBoundingBox(true);
}

////////////////////////////////////////////////////////////////////////////////

Actor::~Actor() {
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  sceneNode_->detachObject(entity_);
  sceneMgr->destroyEntity(entity_);
}

////////////////////////////////////////////////////////////////////////////////

bool Actor::move(const MovementDirection& newDirection,
                 const vector<Actor*>& actors) {
  // Compute new position
  Ogre::Vector3 tmp = position_;
  switch(newDirection) {
  case kUp: tmp.x -= DEFAULT_MOVE_SPEED; break;
  case kDown: tmp.x += DEFAULT_MOVE_SPEED; break;
  case kLeft: tmp.z += DEFAULT_MOVE_SPEED; break;
  case kRight: tmp.z -= DEFAULT_MOVE_SPEED; break;
  default: break;
  }

  // Collision detection
  bool validMove = true;
  for (size_t i = 0; i < actors.size(); i++) {
    if (sceneNode_ != actors[i]->sceneNode_ && fabs(tmp.z - actors[i]->position_.z) < DEFAULT_COLLISION_RAD*2){
      if(CylinderHit(tmp, actors[i]->position_)) {
        validMove = false;
      }
    }
  }

  // If no collisions actually move
  if (validMove) {
    bool wasTranslated = false;

    switch(newDirection) {
    case kUp:
      {
        if (tmp.x > -LEVEL_WIDTH / 2) {
          sceneNode_->translate(Vector3(-DEFAULT_MOVE_SPEED, 0, 0));
          wasTranslated = true;
        }
      }
      break;
    case kDown:
      {
        if (tmp.x < LEVEL_WIDTH / 2) {
          sceneNode_->translate(Vector3(DEFAULT_MOVE_SPEED, 0, 0));
          wasTranslated = true;
        }
      }
      break;
    case kLeft:
      {
        sceneNode_->translate(Vector3(0, 0, DEFAULT_MOVE_SPEED));
        
        /*if (isFacingRight_) {
          sceneNode_->yaw(Ogre::Degree(180));
          isFacingRight_ = false;
        }*/
        wasTranslated = true;
      }
      break;
    case kRight:
      {
        sceneNode_->translate(Vector3(0, 0, -DEFAULT_MOVE_SPEED));

/*        if (!isFacingRight_) {
          sceneNode_->yaw(Ogre::Degree(180));
          isFacingRight_ = true;
        }*/
        wasTranslated = true;
      }
      break;
    default: break;
    }
		
 		sceneNode_->yaw(Ogre::Degree(90*(newDirection-direction_)));
    direction_ = newDirection;


    if (wasTranslated) {
      position_ = tmp;
    }
  }
    
  return validMove;
}


bool Actor::onDamage(float damage){
  stats_.subHealth(damage);
  if(stats_.getHealth() <= 0){
    onDeath();
    return true;
  }
  return false;
}

void Actor::onDeath()
{
  //in leiu of actually destroying it, right now, we'll just move it.
  position_[1]+= 10000;
  position_[2]+= 10000;
  sceneNode_->translate(Vector3(0,10000,10000));
}

void Actor::attack(std::vector<Actor*> &actors){
  //find out where the damage box is, based on direction facing
  //Ogre::Vector3 damPos = position_;
	int vert = 0;
	int horiz = 0;
	
	switch(direction_) {
  case kUp: vert = -1; break;
  case kDown: vert = 1; break;
  case kLeft: horiz = 1; break;
  case kRight: horiz = -1; break;
  default: break;
  }
	
	
	Ogre::Vector3 damagePos = Ogre::Vector3(position_[0] + ( 2*DEFAULT_ATTACK_BOX*vert ),
								                       position_[1], position_[2] + ( 2*DEFAULT_ATTACK_BOX*horiz ));
	/*damagePos[0] = position[0] + ( 2*CollisionSideLength*vert );
	damagePos[1] = position[1];
	damagePos[2] = position[2] + ( 2*CollisionSideLength*horiz );*/
    
  //x and z value for each point of the damage box
  float damage_box[4][2];
  damage_box[0][0] = damagePos[0] - DEFAULT_ATTACK_BOX/2;
  damage_box[0][1] = damagePos[2] + DEFAULT_ATTACK_BOX/2;

  damage_box[1][0] = damagePos[0] - DEFAULT_ATTACK_BOX/2;
  damage_box[1][1] = damagePos[2] - DEFAULT_ATTACK_BOX/2;
    
  damage_box[2][0] = damagePos[0] + DEFAULT_ATTACK_BOX/2;
  damage_box[2][1] = damagePos[2] + DEFAULT_ATTACK_BOX/2;
    
  damage_box[3][0] = damagePos[0] + DEFAULT_ATTACK_BOX/2;
  damage_box[3][1] = damagePos[2] - DEFAULT_ATTACK_BOX/2;
    
  for(unsigned int i = 0; i < actors.size(); i++)
  {
  	if (sceneNode_ != actors[i]->sceneNode_ && fabs(position_.z - actors[i]->position_.z) < DEFAULT_COLLISION_RAD*5){
	    //get actor's hit box
	    float actor_box[4][2];
	    actor_box[0][0] = actors[i]->position_[0] - DEFAULT_COLLISION_RAD/2;
	    actor_box[0][1] = actors[i]->position_[2] + DEFAULT_COLLISION_RAD/2;

	    actor_box[1][0] = actors[i]->position_[0] - DEFAULT_COLLISION_RAD/2;
	    actor_box[1][1] = actors[i]->position_[2] - DEFAULT_COLLISION_RAD/2;
	      
	    actor_box[2][0] = actors[i]->position_[0] + DEFAULT_COLLISION_RAD/2;
	    actor_box[2][1] = actors[i]->position_[2] + DEFAULT_COLLISION_RAD/2;
	      
	    actor_box[3][0] = actors[i]->position_[0] + DEFAULT_COLLISION_RAD/2;
	    actor_box[3][1] = actors[i]->position_[2] - DEFAULT_COLLISION_RAD/2;
	      
	    for(int j = 0; j < 4; j++)          
	    {
	      if(damage_box[j][1] <= actor_box[0][1] &&
	          damage_box[j][1] >= actor_box[1][1] &&
	          damage_box[j][0] >= actor_box[0][0] &&
	          damage_box[j][0] <= actor_box[2][0]
	          )
	      {
	        //if we get here, the actor is hit by the attack
	        if(actors[i]->onDamage(5)) {//////////////////////////////////////////////////////////////////////////////////////////////
	          delete actors[i];
	          actors.erase (actors.begin()+i);
	        }
	      }
	    }
	  }
  }
}
