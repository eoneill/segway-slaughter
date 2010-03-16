////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Class which defines a character in the game
////////////////////////////////////////////////////////////////////////////////
#include "Actor.h"
#include "Locator.h"
#include <iostream>
using namespace std;
using namespace Ogre;

bool SquareHit(const Ogre::Vector3& pos1,
               const Ogre::Vector3& pos2,
               const double width = DEFAULT_BBOX_WIDTH / 2) {
  double left1 = pos1.x - width, left2 = pos2.x - width;
  double right1 = pos1.x + width, right2 = pos2.x + width;
  double top1 = pos1.z - width, top2 = pos2.z - width;
  double bottom1 = pos1.z + width, bottom2 = pos2.z + width;

  if (bottom1 < top2 || top1 > bottom2 ||
      right1 < left2 || left1 > right2) {
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

Actor::Actor(const std::string& entityName,
             const std::string& entityMesh,
             const Status& stats,
             const Ogre::Vector3& position)
  : position_(position),
    direction_(kRight),
    stats_(stats),
    speed_(DEFAULT_MOVE_SPEED)
{
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  entity_ = sceneMgr->createEntity(entityName, entityMesh);
  pSystem_ = sceneMgr->createParticleSystem( entityName+" particle","Blood");
  pSystem_->getEmitter(0)->setEnabled(false);
  pSystem_->getEmitter(0)->setDuration(0.5);

  sceneNode_ = sceneMgr->getRootSceneNode()->createChildSceneNode(position_);
  sceneNode_->attachObject(entity_);
  sceneNode_->attachObject(pSystem_);
  sceneNode_->setScale(40,40,40);
  actorSFX_ = new audSFX();
  actorSFX_->audLoadDir("resources/audio/sfx","wav");
}

////////////////////////////////////////////////////////////////////////////////

Actor::~Actor() {
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  sceneNode_->detachObject(entity_);
  sceneNode_->detachObject(pSystem_);
  sceneMgr->destroyEntity(entity_);
  sceneMgr->destroyParticleSystem(pSystem_);
  sceneMgr->destroySceneNode(sceneNode_);
}

////////////////////////////////////////////////////////////////////////////////

bool Actor::move(const MovementDirection& newDirection,
                 const vector<Actor*>& actors) {
  // Compute new position
  Ogre::Vector3 tmp = position_;
  switch(newDirection) {
  case kUp: tmp.x -= speed_; break;
  case kDown: tmp.x += speed_; break;
  case kLeft: tmp.z += speed_; break;
  case kRight: tmp.z -= speed_; break;
  default: break;
  }

  // Collision detection
  bool validMove = true;
  for (size_t i = 0; i < actors.size(); i++) {
    if (sceneNode_ != actors[i]->sceneNode_){
      if(SquareHit(tmp, actors[i]->position_)) {
        validMove = false;
      }
    }//Acute (a), obtuse (b), and straight (c) angles. Here, a and b are supplementary angles.

  }

  // If no collisions actually move
  if (validMove) {
    bool wasTranslated = false;

    switch(newDirection) {
    case kUp:
      {
        if (tmp.x > -LEVEL_WIDTH / 2) {
          sceneNode_->translate(Vector3(-speed_, 0, 0));
          wasTranslated = true;
        }
      }
      break;
    case kDown:
      {
        if (tmp.x < LEVEL_WIDTH / 2) {
          sceneNode_->translate(Vector3(speed_, 0, 0));
          wasTranslated = true;
        }
      }
      break;
    case kLeft:
      {
        sceneNode_->translate(Vector3(0, 0, speed_));
        wasTranslated = true;
      }
      break;
    case kRight:
      {
        sceneNode_->translate(Vector3(0, 0, -speed_));
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
  pSystem_->getEmitter(0)->setEnabled(true);
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
  stats_.setState(dead);
  actorSFX_->audPlay("male_scream.wav");
}

void Actor::attack(std::vector<Actor*> &actors){
  //find out where the damage box is, based on direction facing
  int vert = 0;
  int horiz = 0;
        
  switch(direction_) {
		case kUp: vert = -1; break;
		case kDown: vert = 1; break;
		case kLeft: horiz = 1; break;
		case kRight: horiz = -1; break;
		default: break;
  }
        
        
  Ogre::Vector3 damagePos = Ogre::Vector3(position_[0] + ( 2*attackBox_*vert ),
                                          position_[1], position_[2] + ( 2*attackBox_*horiz ));

	bool isPlayer = false;
	if (sceneNode_ == actors[0]->sceneNode_)
	{
		isPlayer = true;
	}

	unsigned int start = 1;
	unsigned int end = actors.size();
	
	if(isPlayer == false)
	{
		start = 0;
		end = 1;
	}

	for (size_t i = start; i < end; i++) {
	  if (sceneNode_ != actors[i]->sceneNode_) {
	    if (SquareHit(damagePos, actors[i]->position_, attackBox_)) {
	      if (actors[i]->onDamage(damage_)) {
          stats_.addScore(10);
	      }
	    }
	  }
  }
}


