////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   Various items seen throughout the game
////////////////////////////////////////////////////////////////////////////////
#include "Item.h"

int Item::id_ = 0;
const double ITEM_SCALE = 50;

////////////////////////////////////////////////////////////////////////////////
// Item

Item::Item(const std::string& entityName,
           const std::string& entityMesh,
           const Ogre::Vector3& position)
  : position_(position),
    sceneNode_(0),
    entity_(0)
{
  // Add a unique ID to the entity name
  std::stringstream ss;
  ss << entityName << id_++;
  std::string entityNameUniq = ss.str();
	
  // Create an entity from the mesh
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  entity_ = sceneMgr->createEntity(entityNameUniq, entityMesh);

  // Attach to item to a scene node for rendering
  sceneNode_ = sceneMgr->getRootSceneNode()->createChildSceneNode(position_);
  sceneNode_->attachObject(entity_);
  sceneNode_->setScale(ITEM_SCALE, ITEM_SCALE, ITEM_SCALE);
}

////////////////////////////////////////////////////////////////////////////////

Item::~Item() {
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  sceneNode_->detachObject(entity_);
  sceneMgr->destroyEntity(entity_);
}

////////////////////////////////////////////////////////////////////////////////
// Brawndo

Brawndo::Brawndo(const Ogre::Vector3& position)
  : Item("Brawndo", "brawndo.mesh", position)
{ }

////////////////////////////////////////////////////////////////////////////////
// Pizza

Pizza::Pizza(const Ogre::Vector3& position)
  : Item("Pizza", "pizza.mesh", position)
{ }
