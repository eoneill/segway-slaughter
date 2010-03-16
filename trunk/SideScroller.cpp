#include "SideScroller.h"
#include "CasinoLevel.h"
#include "Paradise.h"
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
  delete streetMusic_;
  delete streetSFX_;

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
  player->setDamage(0.05);
  player->setAttackBox(100);

  actors.push_back(player);
  
  //make some sample enemies
  srand ( time(NULL) );
  //Set the number of enemies so that spawnBehind knows what to do
	NumEnemies_ = 70;
  for(int i = 0; i < NumEnemies_; i++){
    char EntName[40] = "Mobster";
    sprintf(EntName,"mobster%d",i);
    Actor* temp = new Actor(EntName,"mobster.mesh", Status(25),
    	                      Ogre::Vector3(rand() % LEVEL_WIDTH - LEVEL_WIDTH/2,0,-(rand() % 60000+2000)));
    SceneNode * tempSceneNode = temp->getSceneNode();
		tempSceneNode->yaw(Ogre::Degree(180));
		temp->setDamage(0.01);
		temp->setSpeed(0.85);
		temp->setAttackBox(75);
    
    actors.push_back(temp);
  }
	
  // Items
  for (int i = 0; i < 20; i++) {
    Item* tmp;
    if (i % 2 == 0) {
      tmp = new Brawndo(Ogre::Vector3(rand() % LEVEL_WIDTH - LEVEL_WIDTH / 2, 0, -(rand() % 60000+2000)));
    } else {
      tmp = new Pizza(Ogre::Vector3(rand() % LEVEL_WIDTH - LEVEL_WIDTH / 2, 0, -(rand() % 60000+2000)));
    }
    items.push_back(tmp);
  }

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


  //AUDIO
  streetMusic_ = new audBackground(1);
  streetMusic_->audLoadDir("resources/audio/music/Street_Level","wav");
  streetMusic_->audPlay();

  streetSFX_ = new audSFX();
  streetSFX_->audLoadDir("resources/audio/sfx","wav");
  
  bossFight = false;
}


////////////////////////////////////////////////////////////////////////////////

bool SideScroller::isDone() {
  return isDone_;
}

////////////////////////////////////////////////////////////////////////////////

GameState* SideScroller::update(const Ogre::Real& timeSinceLastFrame) {
	removeDead();
	
	if(!bossFight){
		spawnBehind(actors, NumEnemies_);
	}

  player->subEffectTime(timeSinceLastFrame);

  Ogre::Root* root_ = getRoot();
  Camera* mCamera = root_->getSceneManager("Default SceneManager")->getCamera("MyCamera");
  InputSystem* is = Locator::getInput();
  
  //cool down for the chainsaw
  if(player->chainsawHeat >= 0)
	  player->chainsawHeat -=0.015;
	if(player->chainsawHeat == 0)
	  player->chainsawHeat = 0;

  //Sound stuff
  if (!streetSFX_->audIsPlaying("chainsaw_idle.wav") &&
      !streetSFX_->audIsPlaying("chainsaw_attack.wav"))
    streetSFX_->audPlay("chainsaw_idle.wav");
		
	//update AI
	AIManager(actors);

  //Move player up, but with constraints
  if (is->isKeyDown(OIS::KC_UP)) {
      if (!streetSFX_->audIsPlaying("segway_ride.wav"))
        streetSFX_->audPlay("segway_ride.wav");
      player->move(kUp, actors, items);
  }
  //Move player down, but with constraints
  if (is->isKeyDown(OIS::KC_DOWN)) {
      if (!streetSFX_->audIsPlaying("segway_ride.wav"))
        streetSFX_->audPlay("segway_ride.wav");
      player->move(kDown, actors, items);
  }
  //move player left
    if (is->isKeyDown(OIS::KC_LEFT)) {
      if (!streetSFX_->audIsPlaying("segway_ride.wav"))
        streetSFX_->audPlay("segway_ride.wav");
      if (!bossFight || (bossFight && player->getPosition()[2] <= -62000))
      { 
		    if(player->move(kLeft, actors, items) && !bossFight)
		    {
		      mCamera->move(Vector3(0,0,player->getSpeed()));
		    }
		  }
    }
    //move player right
    if (is->isKeyDown(OIS::KC_RIGHT)) {
      if (!streetSFX_->audIsPlaying("segway_ride.wav"))
        streetSFX_->audPlay("segway_ride.wav");
      if (player->getPosition()[2] >= -64000)
      {
		  	if(player->move(kRight, actors, items) && !bossFight)
		  	{
		      mCamera->move(Vector3(0,0,-player->getSpeed()));
		    }
		  }
    }

    if (is->isKeyDown(OIS::KC_A)) {
    if(player->chainsawHeat <= MAX_HEAT)
		  player->chainsawHeat +=0.035;
		if(player->chainsawHeat == MAX_HEAT)
		  player->chainsawHeat = MAX_HEAT;
    if(player->chainsawHeat <= MAX_HEAT-1)
	  {
		    if (!streetSFX_->audIsPlaying("chainsaw_attack.wav"))
		      streetSFX_->audPlay("chainsaw_attack.wav");
		    if(player->attack(actors))
		    	{
		    		isDone_ = true;
				    return new Paradise;
		    	}
		    hud_->updateScore(player->getScore());
    	}
    }
    else{
      player->stopBlood();
      for(unsigned int i = 0; i < actors.size(); i++)
        actors[i]->stopBlood();
    }
    
		//get to the end, trigger a boss fight
	  if (player->getPosition()[2] <= -63000 && bossFight == false) {
	    //isDone_ = true;
	    //return new CasinoLevel;
	    bossFight = true;
	    NumEnemies_++;
			Actor* temp = new Actor("Boss","mob_boss.mesh", Status(100),
				                      Ogre::Vector3(rand() % LEVEL_WIDTH - LEVEL_WIDTH/2,0, actors[0]->getPosition()[2] - 1000));
			SceneNode * tempSceneNode = temp->getSceneNode();
			tempSceneNode->yaw(Ogre::Degree(180));
			temp->setDamage(0.01);
			temp->setSpeed(1.1);
			temp->setAttackBox(75);
			temp->setState(attack);
			temp->isBoss = true;

			actors.push_back(temp);

	  }

  if (is->isKeyDown(OIS::KC_1)) {
    isDone_ = true;
    return new Paradise;
  }
  
  if(player->getState() == dead)
  {
  	isDone_ = true;
    return new MainMenu;
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
  hud_->updateHeat(player->chainsawHeat/MAX_HEAT);

  unsigned int millis = timer_.getMilliseconds() / 1000;

  if (millis >= 1) {
    timeLeft_ -= millis;
    hud_->updateTimeLeft(timeLeft_ / 60, timeLeft_ % 60);
    timer_.reset();
  }
	
  return NULL;
}

