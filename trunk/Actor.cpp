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
