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
  nodeName_ = entityNameUniq;

  // Create an entity from the mesh
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  entity_ = sceneMgr->createEntity(entityNameUniq, entityMesh);

  // Attach to item to a scene node for rendering
  sceneNode_ = sceneMgr->getRootSceneNode()->createChildSceneNode(position_);
  sceneNode_->attachObject(entity_);

	// Set the initial status effects
	statEffects_["Health"] = 0;
	statEffects_["Speed"]  = 0;
	statEffects_["Time"]   = 0;
}

////////////////////////////////////////////////////////////////////////////////

Item::~Item() {
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  if (sceneMgr->hasEntity(nodeName_)) {
    sceneNode_->detachObject(entity_);
    sceneMgr->destroyEntity(entity_);
    sceneMgr->destroySceneNode(sceneNode_);
  }
}

////////////////////////////////////////////////////////////////////////////////

double Item::getStatusEffect(const std::string& name) {
	if (statEffects_.find(name) == statEffects_.end()) {
		return 0;
	}

	return statEffects_[name];
}

////////////////////////////////////////////////////////////////////////////////

void Item::scaleMesh(const double& scaleFactor) {
  sceneNode_->setScale(scaleFactor, scaleFactor, scaleFactor);
}

////////////////////////////////////////////////////////////////////////////////
// Brawndo

Brawndo::Brawndo(const Ogre::Vector3& position)
  : Item("Brawndo", "brawndo.mesh", position)
{
	statEffects_["Speed"] = 1.0;
	statEffects_["Time"]  = 10; // 10 seconds
  scaleMesh(100);
}

////////////////////////////////////////////////////////////////////////////////

double Brawndo::getWidth() {
  return 80;
}

////////////////////////////////////////////////////////////////////////////////
// Pizza

Pizza::Pizza(const Ogre::Vector3& position)
  : Item("Pizza", "pizza.mesh", position)
{
	statEffects_["Health"] = 100;
	statEffects_["Time"] = 0;
  scaleMesh(50);
}

////////////////////////////////////////////////////////////////////////////////

double Pizza::getWidth() {
  return 100;
}

////////////////////////////////////////////////////////////////////////////////
// Claymore

Claymore::Claymore(const Ogre::Vector3& position)
  : Item("Claymore", "claymore.mesh", position)
{
	statEffects_["Health"] = -20;
	statEffects_["Time"] = 0;
  scaleMesh(50);
}

////////////////////////////////////////////////////////////////////////////////

double Claymore::getWidth() {
  return 100;
}
