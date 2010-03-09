#include "Item.h"

Item::Item(const std::string& entityName,
           const std::string& entityMesh,
           const Ogre::Vector3& position)
  : position_(position),
    sceneNode_(0),
    entity_(0)
{
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  entity_ = sceneMgr->createEntity(entityName, entityMesh);

  sceneNode_ = sceneMgr->getRootSceneNode()->createChildSceneNode(position_);
  sceneNode_->attachObject(entity_);
  sceneNode_->setScale(10, 10, 10);
}

////////////////////////////////////////////////////////////////////////////////

Item::~Item() {
  Ogre::SceneManager* sceneMgr = Locator::getSceneManager();
  sceneNode_->detachObject(entity_);
  sceneMgr->destroyEntity(entity_);
}
