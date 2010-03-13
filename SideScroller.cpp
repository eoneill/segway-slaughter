#include "SideScroller.h"
#include "CasinoLevel.h"
#include "MainMenu.h"
#include "AIManager.h"
#include "Item.h"
#include <iostream>

using namespace std;
using namespace Ogre;


SideScroller::SideScroller()
  : hud_(0),
    isDone_(false),
    timeLeft_(TIME_PER_LEVEL)
    //timer_(new Ogre::Timer())
{ }

////////////////////////////////////////////////////////////////////////////////

SideScroller::~SideScroller() {
  for(unsigned int i = 0; i < actors.size(); i++) {
  	delete actors[i];
  }
  SceneManager* mSceneMgr = getRoot()->getSceneManager("Default SceneManager");
  mSceneMgr->destroyAllCameras();
  mSceneMgr->destroyAllStaticGeometry();
  mSceneMgr->destroyAllEntities();
  mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
  mSceneMgr->destroyAllParticleSystems();
  mSceneMgr->destroyAllLights();
  mSceneMgr->clearScene();
  getRoot()->getAutoCreatedWindow()->removeAllViewports();

  assert(hud_ != 0);
  delete hud_;
}

///////////////////////////////////////////////////////////////////////////////

void SideScroller::initialize() {
  Ogre::Root* root_ = getRoot();
  SceneManager* mSceneMgr = root_->getSceneManager("Default SceneManager");
  assert(mSceneMgr != 0);

  mSceneMgr->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));
  //mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
  mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);

  //Setup the camera
  Camera *mCamera = mSceneMgr->createCamera("MyCamera");
  mCamera->setNearClipDistance(10);
  //Viewport *mViewport = 
  root_->getAutoCreatedWindow()->addViewport(mCamera);

  //Camera *mCamera = mRoot->getSceneManager("Default SceneManager")->getCamera("MyCamera");
  mCamera->setPosition(2000,1000,0);
  mCamera->setDirection(-1,-.3,0);

  // TEST PLANE
  Plane plane(Vector3::UNIT_Y, 0);
  MeshManager::getSingleton().createPlane("SSground",
                                          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                                          1550,151800,1,1,true,1,1,60,Vector3::UNIT_Z);
  Entity* ent = mSceneMgr->createEntity("SSGroundEntity", "SSground");
  ent->setMaterialName("Suburb/Road");
  ent->setCastShadows(false);

  SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
  node->attachObject(ent);
  node->translate(0,0,1000);

  Plane backplane(Vector3::UNIT_X, 0);
  MeshManager::getSingleton().createPlane("SSBack",
                                          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, backplane,
                                          1550,151800,1,1,true,1,1,60,Vector3::UNIT_Z);
  ent = mSceneMgr->createEntity("SSBuilding1", "SSBack");
  ent->setMaterialName("Urban/Building");
  node = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-750,775,0));
//  node->scale(75,75,75);
  node->attachObject(ent);

  //Player
  player = new Charlie("charlie", "charlie.mesh", Ogre::Vector3(0,0,0));
  player->setDamage(.05);

  actors.push_back(player);
  
  //make some sample enemies
  srand ( time(NULL) );
  //Set the number of enemies so that spawnBehind knows what to do
	NumEnemies_ = 35;
  for(int i = 0; i < NumEnemies_; i++){
    char EntName[40] = "Mobster";
    sprintf(EntName,"mobster%d",i);
    Actor* temp = new Actor(EntName,"brawndo.mesh", Status(25),
    	                      Ogre::Vector3(rand() % LEVEL_WIDTH - LEVEL_WIDTH/2,0,-(rand() % 30000+2000)));
    SceneNode * tempSceneNode = temp->getSceneNode();
		tempSceneNode->yaw(Ogre::Degree(180));
		temp->setDamage(0.01);
		temp->setSpeed(0.85);
    
    actors.push_back(temp);
  }
	

	//Brawndo b(Ogre::Vector3(0,0,0));

  //Static Objects
  ent = mSceneMgr->createEntity("SSPole1", "tel_pole_basic.mesh");
  //ent->setMaterialName("tel_pole_basic");
  StaticGeometry *sg = mSceneMgr->createStaticGeometry("SSPole1");
	for(int i = 0; i < 20; i++)
    sg->addEntity(ent, Ogre::Vector3(-550,0,-6300*i), Quaternion::IDENTITY, Ogre::Vector3(70,70,70));
  sg->setCastShadows(true);
  sg->build();

  // Light
  mSceneMgr->setAmbientLight(ColourValue(0.4, 0.4, 0.4));
  Light *light = mSceneMgr->createLight("Light1");
  light->setType(Light::LT_DIRECTIONAL);
  light->setDirection(Vector3(-1, -1, -1));
  light->setPosition(Vector3(0, 25000, 25000));
  light->setDiffuseColour(0.3, 0.3, 0.3);
  light->setSpecularColour(0.3, 0.3, 0.3);
    

  //SKYPLANE
//  Plane skyplane(Vector3::UNIT_X, -1550);
//  mSceneMgr->setSkyPlane(true, skyplane, "Examples/CloudySky");
  //////////////************
  hud_ = new HUD();
  hud_->updateTimeLeft(timeLeft_ / 60, timeLeft_ % 60);
}


////////////////////////////////////////////////////////////////////////////////

bool SideScroller::isDone() {
  return isDone_;
}

////////////////////////////////////////////////////////////////////////////////

GameState* SideScroller::update(const Ogre::Real& timeSinceLastFrame) {
	removeDead();
	
	spawnBehind(actors, NumEnemies_);

  Ogre::Root* root_ = getRoot();
  Camera* mCamera = root_->getSceneManager("Default SceneManager")->getCamera("MyCamera");
  InputSystem* is = Locator::getInput();
		
	//update AI
	AIManager(actors);

  //Move player up, but with constraints
  if (is->isKeyDown(OIS::KC_UP)) {
 		player->move(kUp, actors);
  }
  //Move player down, but with constraints
  if (is->isKeyDown(OIS::KC_DOWN)) {
  	player->move(kDown, actors);
  }
  //move player left
    if (is->isKeyDown(OIS::KC_LEFT)) {
	    if(player->move(kLeft, actors))
      {
        mCamera->move(Vector3(0,0,DEFAULT_MOVE_SPEED));
      }
    }
    //move player right
    if (is->isKeyDown(OIS::KC_RIGHT)) {
    	if(player->move(kRight, actors))
    	{
	      mCamera->move(Vector3(0,0,-DEFAULT_MOVE_SPEED));
      }
    }

    if (is->isKeyDown(OIS::KC_A)) {
      player->attack(actors);
      hud_->updateScore(player->getScore());
    }
    else{
      player->stopBlood();
      for(unsigned int i = 0; i < actors.size(); i++)
        actors[i]->stopBlood();
    }
    
/*
	  if (player.position[2] <= -33000) {
	    isDone_ = true;
	    return new CasinoLevel;
	  }*/

  if (is->isKeyDown(OIS::KC_1)) {
    isDone_ = true;
    return new CasinoLevel;
  }
  if (is->isKeyDown(OIS::KC_ESCAPE)) {
    isDone_ = true;
    return new MainMenu;
  }
  if (timeLeft_ <= 0) {
    isDone_ = true;
    return new MainMenu;
  }

  hud_->updateHealth(player->getHealth() / player->getMaxHealth());

  unsigned int millis = timer_.getMilliseconds() / 1000;

  if (millis >= 1) {
    timeLeft_ -= millis;
    hud_->updateTimeLeft(timeLeft_ / 60, timeLeft_ % 60);
    timer_.reset();
  }
	
  return NULL;
}

