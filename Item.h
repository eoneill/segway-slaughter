////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Various items seen throughout the game
////////////////////////////////////////////////////////////////////////////////
#ifndef ITEM_H__
#define ITEM_H__

#include <map>
#include <string>

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

  Ogre::Vector3 getPosition() {
    return position_;
  }

  virtual double getWidth() {
    return 50;
  }

  double getStatusEffect(const std::string& name);
protected:
  void scaleMesh(const double& scaleFactor);
  std::map<std::string, double> statEffects_;
private:
  Ogre::Vector3    position_;
  Ogre::SceneNode* sceneNode_;
  Ogre::Entity*    entity_;
  std::string      nodeName_;
  static int       id_;
};

////////////////////////////////////////////////////////////////////////////////
// Class: Brawndo
//
// Causes the character to speed up and damage people by running them over
class Brawndo : public Item {
public:
  Brawndo(const Ogre::Vector3& position);
  double getWidth();
};

////////////////////////////////////////////////////////////////////////////////
// Class: Pizza
//
// Restores a little bit of health when eaten
class Pizza : public Item {
public:
  Pizza(const Ogre::Vector3& position);
  double getWidth();
};

////////////////////////////////////////////////////////////////////////////////
// Class: Claymore
//
// Deals health damage when stepped on
class Claymore : public Item {
public:
  Claymore(const Ogre::Vector3& position);
  double getWidth();
};

#endif // ITEM_H__
