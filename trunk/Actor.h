////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Class which defines a character in the game
////////////////////////////////////////////////////////////////////////////////
#ifndef ACTOR_H_
#define ACTOR_H_

#include <string>
#include <vector>
#include <iostream>
#include <Ogre.h>
#include "audio/audio.h"
#include "Item.h"

////////////////////////////////////////////////////////////////////////////////
// Constants
enum MovementDirection {
  kDown,
  kRight,
  kUp,
  kLeft
};

enum actorState {
  dead,
  idle,
  attack,
  flee
};

const int LEVEL_WIDTH = 1000;

// The number of points a player receives for each enemy kill
const int ENEMY_KILL_POINTS = 10;

// Affects the player's maximum amount of health as well as initial health
const float DEFAULT_MAX_HEALTH = 100;

// The movement speed of the player
const float DEFAULT_MOVE_SPEED = 1.33;

// The default bounding box side length
const float DEFAULT_COLLISION_RAD = 100;

// The default attack box size
const float DEFAULT_ATTACK_BOX = 100;

// The default bounding box width
const double DEFAULT_BBOX_WIDTH = 100;

////////////////////////////////////////////////////////////////////////////////
// Class: Status
//
// Represents the current status of an actor
class Status {
public:
  Status(const float& initialHealth)
  : health_(initialHealth),
    maxHealth_(initialHealth),
    score_(0),
    state_(idle)
  { }

  bool isDead() { return (health_ <= 0); }

  float getMaxHealth() const { return maxHealth_; }

  float getHealth() const { return health_; }
  void  addHealth(float a) {
    health_ += a;
    if (health_ > maxHealth_) {
      health_ = maxHealth_;
    }
  }
  void  subHealth(float a) { health_ -= a; }

  int getScore() const { return score_; }
  void addScore(int a) { score_ += a; }
  void subScore(int a) { score_ -= a; }
  
  actorState getState() { return state_; }
  void setState(actorState state) { state_ = state; }
  
private:
  float health_;
  float maxHealth_;
  int   score_;
  actorState state_;
};

////////////////////////////////////////////////////////////////////////////////
// Class: Actor
//
// Movable meshes
class Actor {
public:
  Actor(const std::string& entityName,
        const std::string& entityMesh,
        const Status& stats,
        const Ogre::Vector3& position = Ogre::Vector3::ZERO);

  virtual ~Actor();

  //////////////////////////////////////////////////////////////////////////////
  // Function: move
  //
  // Returns true if there was a collision when attempt to move this actor,
  // false otherwise.
  virtual bool move(const MovementDirection& direction,
                    const std::vector<Actor*>& actors,
                    std::vector<Item*>& items);
  virtual bool move(const MovementDirection& direction,
                    const std::vector<Actor*>& actors);

  //////////////////////////////////////////////////////////////////////////////
  // Function: attack
  //
  // Attempt to perform an attack on nearby entities
  bool attack(std::vector<Actor*>& actors);

  //////////////////////////////////////////////////////////////////////////////
  // Function: stopBlood
  //
  // Stops the blood from spraying from an attacked enemy
  void stopBlood() {} 

  //////////////////////////////////////////////////////////////////////////////
  // Function: attack
  //
  // When the character attacks
  bool attack();

  //////////////////////////////////////////////////////////////////////////////
  // Function: onDamage
  //
  // Handles an attack on this actor
  bool onDamage(float damage);
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: onDeath
  //
  // Handles the death of this actor
  void onDeath();

  //////////////////////////////////////////////////////////////////////////////
  // Function: getScore
  //
  // Returns the current score for this actor
  int getScore() const {
    return stats_.getScore();
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: getState
  //
  // Returns the current state for this actor
  actorState getState() {
    return stats_.getState();
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: setState
  //
  // sets the current state for this actor
  void setState(actorState s) {
    stats_.setState(s);
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: getHealth
  //
  // Returns the current health of the character
  float getHealth() const {
    return stats_.getHealth();
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: getMaxHealth
  //
  // Returns the maximum possible health for this actor
  float getMaxHealth() const {
    return stats_.getMaxHealth();
  }
  
  Status& getStatus() {
    return stats_;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: getPosition
  //
  // Returns the current position of the character
  Ogre::Vector3 getPosition(){return position_;}

  //////////////////////////////////////////////////////////////////////////////
  // Function: getDirection
  //
  // Returns the current direction of the character
  MovementDirection getDirection(){return direction_;}
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: setDirection
  //
  // Sets the current direction of the character
  void setDirection(MovementDirection d){direction_ = d;}
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: getSceneNode
  //
  // Returns the sceneNode_ of the character
  Ogre::SceneNode* getSceneNode(){return sceneNode_;}

  //////////////////////////////////////////////////////////////////////////////
  // Function: addSpeed
  //
  // Ramp up speed like Charlie just took a big swig of Brawndo cola
  void addSpeed(const double& speed) {
    speed_ += speed;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: setSpeed
  //
  // Sets the movement speed of this actor
  void setSpeed(const double& speed) {
    speed_ = speed;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: getSpeed
  //
  // Returns the current speed of this actor
  double getSpeed() const {
    return speed_;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: getDamage
  //
  // Sets the damage of this actor
  double getDamage() {
    return damage_;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: setDamage
  //
  // Sets the damage of this actor
  void setDamage(const double& damage) {
    damage_ = damage;
  }
  //////////////////////////////////////////////////////////////////////////////
  // Function: setAttackBox
  //
  // Sets the size of the attack box for this character
  void setAttackBox(const double& size) {
    attackBox_ = size;
  }
  
  
  bool isBoss;
protected:
  Ogre::Vector3     position_;
  Ogre::SceneNode*  sceneNode_;
  Ogre::Entity*     entity_;
  Ogre::ParticleSystem* pSystem_;
  MovementDirection direction_;
  Status            stats_;
  double            speed_;
  double            damage_;
  //audSFX *          actorSFX_;
  float             attackBox_;
};

class Charlie : public Actor {
public:
  Charlie(const std::string& entityName,
          const std::string& entityMesh,
          const Ogre::Vector3& position = Ogre::Vector3::ZERO)
  : Actor(entityName, entityMesh, Status(400), position)
  {
    direction_ = kRight;
    sceneNode_->yaw(Ogre::Degree(-180));
  }
};

#endif // ACTOR_H_
