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

////////////////////////////////////////////////////////////////////////////////
// Constants
enum MovementDirection {
  kDown,
  kRight,
  kUp,
  kLeft
};

const int LEVEL_WIDTH = 1000;

// The number of points a player receives for each enemy kill
const int ENEMY_KILL_POINTS = 10;

// Affects the player's maximum amount of health as well as initial health
const float DEFAULT_MAX_HEALTH = 100;

// The movement speed of the player
const float DEFAULT_MOVE_SPEED = 1.33;

// The default bounding box side length
const float DEFAULT_COLLISION_RAD = 50;

// The default attack box size
const float DEFAULT_ATTACK_BOX = 50;

////////////////////////////////////////////////////////////////////////////////
// Class: Status
//
// Represents the current status of an actor
class Status {
public:
  Status(const float& initialHealth)
  : health_(initialHealth),
    maxHealth_(initialHealth),
    score_(0)
  { }

  bool isDead() { return (health_ <= 0); }

  float getMaxHealth() const { return maxHealth_; }

  float getHealth() const { return health_; }
  void  addHealth(float a) { health_ += a; }
  void  subHealth(float a) { health_ -= a; }

  int getScore() { return score_; }
  void addScore(int a) { score_ += a; }
  void subScore(int a) { score_ -= a; }
private:
  float health_;
  float maxHealth_;
  int   score_;
};

////////////////////////////////////////////////////////////////////////////////
// Function: CylinderHit
//
// Returns true if the cylinders around the two points intersect, false
// otherwise.
bool CylinderHit(const Ogre::Vector3& pos1,
                 const Ogre::Vector3& pos2);

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
                    const std::vector<Actor*>& actors);

  //////////////////////////////////////////////////////////////////////////////
  // Function: attack
  //
  // Attempt to perform an attack on nearby entities
  void attack(std::vector<Actor*>& actors);

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
    return 0;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Function: getHealth
  //
  // Returns the current health of the character
  float getHealth() const {
    return DEFAULT_MAX_HEALTH;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: getMaxHealth
  //
  // Returns the maximum possible health for this actor
  float getMaxHealth() const {
    return DEFAULT_MAX_HEALTH;
  }
private:
  Ogre::Vector3     position_;
  Ogre::SceneNode*  sceneNode_;
  Ogre::Entity*     entity_;
  MovementDirection direction_;
  Status            stats_;
};

#endif // ACTOR_H_
