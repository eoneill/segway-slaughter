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
    speed_(DEFAULT_MOVE_SPEED),
    deadTime_(0),
    nodeName_(entityName)
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
  isBoss = false;
  chainsawHeat = 0;
  
  //actorSFX_ = new audSFX();
  //actorSFX_->audLoadDir("resources/audio/sfx","wav");
}

////////////////////////////////////////////////////////////////////////////////

Actor::~Actor() {
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();

  if (sceneMgr->hasEntity(nodeName_)) {
    sceneNode_->detachObject(entity_);
    sceneMgr->destroyEntity(entity_);
    sceneNode_->detachObject(pSystem_);
    sceneMgr->destroyParticleSystem(pSystem_);
    sceneMgr->destroySceneNode(sceneNode_); // Always do this last!
  }
}

////////////////////////////////////////////////////////////////////////////////

void Actor::update(const double& secsSinceLastUpdate) {
  // If there's time left
  if (timedEffect_.getTime() > 0) {
    // Subtract the number of seconds
    timedEffect_.subTime(secsSinceLastUpdate);
    
    // If there is now no time left
    if (timedEffect_.getTime() <= 0) {
      // Reverse the status effect (Brawndo)
      if (timedEffect_.getEffect() == "Speed") {
        speed_ -= timedEffect_.getValue();
      }
    }
  }
  
  mAnimState->addTime(secsSinceLastUpdate);
  mAnimState2->addTime(secsSinceLastUpdate);
  if(isEnemy)
    mAnimState3->addTime(secsSinceLastUpdate);	// for goblin
  
  timeSinceLast_ = secsSinceLastUpdate;
}

////////////////////////////////////////////////////////////////////////////////

bool Actor::move(const MovementDirection& newDirection,
                 const vector<Actor*>& actors,
                 vector<Item*>& items) {
  for (vector<Item*>::iterator it = items.begin();
       it != items.end();) {
    if (SquareHit((*it)->getPosition(),
                  actors[0]->position_,
                  (*it)->getWidth())) {

      double time = (*it)->getStatusEffect("Time");
      
      // A timed status effect
      if (time > 0) {
        // Speed boost (Brawndo)
        if ((*it)->getStatusEffect("Speed") > 0) {
          // If they already have another speed boost
          if (timedEffect_.getTime() > 0) {
            // Just make it last longer
            timedEffect_.setTime(time);
          } else {
            // Otherwise increase their speed
            timedEffect_ = TimedEffect(time, "Speed", (*it)->getStatusEffect("Speed"));
            actors[0]->addSpeed((*it)->getStatusEffect("Speed"));
                  }
             }
      } else {
       actors[0]->addSpeed((*it)->getStatusEffect("Speed"));
      	actors[0]->getStatus().addHealth((*it)->getStatusEffect("Health"));

      }
      

      delete *it;
      it = items.erase(it);
    } else {
      ++it;
    }
  }
  return move(newDirection, actors);
}

////////////////////////////////////////////////////////////////////////////////

bool Actor::move(const MovementDirection& newDirection,
                 const vector<Actor*>& actors)
{
  // Compute new position
  Ogre::Vector3 tmp = position_;
  switch(newDirection) {
  case kUp: tmp.x -= speed_*timeSinceLast_*1000; break;
  case kDown: tmp.x += speed_*timeSinceLast_*1000; break;
  case kLeft: tmp.z += speed_*timeSinceLast_*1000; break;
  case kRight: tmp.z -= speed_*timeSinceLast_*1000; break;
  default: break;
  }

  // Collision detection
  bool validMove = true;
  for (size_t i = 0; i < actors.size(); i++) {
    if (sceneNode_ != actors[i]->sceneNode_ && actors[i]->getState() != dead){
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
          sceneNode_->translate(Vector3(-speed_*timeSinceLast_*1000, 0, 0));
          wasTranslated = true;
        }
      }
      break;
    case kDown:
      {
        if (tmp.x < LEVEL_WIDTH / 2) {
          sceneNode_->translate(Vector3(speed_*timeSinceLast_*1000, 0, 0));
          wasTranslated = true;
        }
      }
      break;
    case kLeft:
      {
        sceneNode_->translate(Vector3(0, 0, speed_*timeSinceLast_*1000));
        wasTranslated = true;
      }
      break;
    case kRight:
      {
        sceneNode_->translate(Vector3(0, 0, -speed_*timeSinceLast_*1000));
        wasTranslated = true;
      }
      break;
    default: break;
    }

    if (wasTranslated) {
      position_ = tmp;
    }
  }

  //Turn character
  sceneNode_->yaw(Ogre::Degree(90*(newDirection-direction_)));
  direction_ = newDirection;
    
  return validMove;
}

////////////////////////////////////////////////////////////////////////////////

bool Actor::onDamage(float damage){
  pSystem_->getEmitter(0)->setEnabled(true);
  stats_.subHealth(damage);
  if(stats_.getHealth() <= 0){
    onDeath();
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

void Actor::onDeath()
{
  //Fall over
  sceneNode_->pitch(Ogre::Degree(90));
  sceneNode_->yaw(Ogre::Degree(180));
  sceneNode_->translate(Vector3(0, 75, 0));
  //Change blood and keep it going
  pSystem_->getEmitter(0)->setDirection(Vector3(0,0,1));
  pSystem_->getEmitter(0)->setParticleVelocity(500,600);
  pSystem_->getEmitter(0)->setDuration(60);
  pSystem_->getEmitter(0)->setEnabled(true);
  stats_.setState(dead);
  deathAnimation();
  //actorSFX_->audPlay("male_scream.wav");
}

////////////////////////////////////////////////////////////////////////////////

bool Actor::attack(std::vector<Actor*> &actors){
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

  bool didBossDie = false;
  
	for (size_t i = start; i < end; i++) {
	  if (sceneNode_ != actors[i]->sceneNode_ && actors[i]->getState() != dead) {
	    if (SquareHit(damagePos, actors[i]->position_, attackBox_)) {
	      if (actors[i]->onDamage(damage_*timeSinceLast_*1000)) {
	      	if (actors[i]->isBoss) {
	      		didBossDie = true;
	      		stats_.addScore(1000);
	      	} else {
            stats_.addScore(10);
          }
        }
      }
    }
  }
  
  return didBossDie;
}


//animation calls
void Actor::attackAnimation()
{
   mAnimState->setEnabled(true);
   mAnimState2->setEnabled(true);
   if(isEnemy)
     mAnimState3->setEnabled(true);
}

void Actor::deathAnimation()
{
  if(!isEnemy)
  {
	mAnimState = entity_->getAnimationState("charlie_body_death");
   mAnimState->setEnabled(true);
   mAnimState->setLoop(false);
   mAnimState2 = entity_->getAnimationState("charlie_chainsaw_death");
   mAnimState2->setEnabled(true);
   mAnimState2->setLoop(false);
  }
  else
  {
  	mAnimState = entity_->getAnimationState("goblin_bat_death");
      mAnimState->setEnabled(true);
      mAnimState->setLoop(false);
      mAnimState2 = entity_->getAnimationState("goblin_body_death");
      mAnimState2->setEnabled(true);
      mAnimState2->setLoop(false);
      mAnimState3 = entity_->getAnimationState("goblin_head_death");
      mAnimState3->setEnabled(true);
      mAnimState3->setLoop(false);
  }
}

void Actor::stopAnimation()
{
	mAnimState->setEnabled(false);
   mAnimState2->setEnabled(false);
   if(isEnemy)
     mAnimState3->setEnabled(false);
}

void Actor::setupAnimation()
{
  if(!isEnemy)
  {
	mAnimState = entity_->getAnimationState("charlie_body_swing");
   mAnimState->setEnabled(false);
   mAnimState->setLoop(true);
   mAnimState2 = entity_->getAnimationState("charlie_chainsaw_swing");
   mAnimState2->setEnabled(false);
   mAnimState2->setLoop(true);
  }
  else
  {
  	mAnimState = entity_->getAnimationState("goblin_bat_animation");
      mAnimState->setEnabled(true);
      mAnimState->setLoop(true);
      mAnimState2 = entity_->getAnimationState("goblin_body_swing");
      mAnimState2->setEnabled(true);
      mAnimState2->setLoop(true);
      mAnimState3 = entity_->getAnimationState("goblin_head_animation");
      mAnimState3->setEnabled(true);
      mAnimState3->setLoop(true);
  }
}


