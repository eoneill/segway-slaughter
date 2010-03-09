////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Various items seen throughout the game
////////////////////////////////////////////////////////////////////////////////
#ifndef ITEM_H__
#define ITEM_H__

#include "Locator.h"

////////////////////////////////////////////////////////////////////////////////
// Class: Item
//
// Represents an item in the game  
class Item {
public:
  Item(const std::string& entityName,
       const std::string& entityMesh,
       const Ogre::Vector3& position);

  virtual ~Item();
private:
  Ogre::Vector3    position_;
  Ogre::SceneNode* sceneNode_;
  Ogre::Entity*    entity_;
};

////////////////////////////////////////////////////////////////////////////////
// Class: Brawndo
//
// Causes the character to speed up and damage people by running them over
class Brawndo : public Item {
public:
  Brawndo()
    : Item("Brawndo", "brawndo.mesh", Ogre::Vector3::ZERO)
  { }
};

////////////////////////////////////////////////////////////////////////////////
// Class: Pizza
//
// Restores a little bit of health when eaten
class Pizza : public Item {
public:
  Pizza()
    : Item("Pizza", "pizza.mesh", Ogre::Vector3::ZERO)
  { }
};

#endif // ITEM_H__
