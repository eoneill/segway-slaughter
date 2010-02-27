#include "SideScroller.h"
#include "CasinoLevel.h"
#include "MainMenu.h"

#include <iostream>

using namespace std;
using namespace Ogre;


SideScroller::SideScroller()
  : isDone_(false)
{ }

////////////////////////////////////////////////////////////////////////////////

SideScroller::~SideScroller() {
  for(unsigned int i = 0; i < actors.size(); i++) {
  	delete actors[i];
  }
  SceneManager* mSceneMgr = getRoot()->getSceneManager("Default SceneManager");
  mSceneMgr->destroyAllCameras();
  mSceneMgr->destroyAllEntities();
  mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
  mSceneMgr->destroyAllParticleSystems();
  mSceneMgr->destroyAllLights();
  mSceneMgr->clearScene();
  getRoot()->getAutoCreatedWindow()->removeAllViewports();

  CEGUI::WindowManager::getSingleton().destroyWindow("HealthBar");
  CEGUI::WindowManager::getSingleton().destroyWindow("HealthText");
}

///////////////////////////////////////////////////////////////////////////////

void SideScroller::initialize() {
  Ogre::Root* root_ = getRoot();
  SceneManager* mSceneMgr = root_->getSceneManager("Default SceneManager");
  assert(mSceneMgr != 0);

  mSceneMgr->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));
  mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);

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
  MeshManager::getSingleton().createPlane("ground",
                                          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                                          150000,150000,20,20,true,1,5,5,Vector3::UNIT_Z);
  Entity* ent = mSceneMgr->createEntity("GroundEntity", "ground");
  ent->setMaterialName("Examples/Rockwall");
  ent->setCastShadows(false);

  SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
  node->attachObject(ent);
  node->translate(0,0,0);

  //Player
  player = new Actor("ninja","ninja.mesh", Ogre::Vector3(0,0,0));
  actors.push_back(player);
  
  //make some sample enemies
   srand ( time(NULL) );
   for(int i = 0; i < 100; i++){
    char EntName[40] = "Robot";
    sprintf(EntName,"robot%d",i);
    Actor* temp = new Actor(EntName,"robot.mesh", Ogre::Vector3(rand() % LEVEL_WIDTH - LEVEL_WIDTH/2,0,-(rand() % 30000+2000)));

    //stats need to be less for the enemies
    //temp->speed_ = 1;
    //temp->MaxHealth_ = 25;
    //temp->CurrentHealth_ = 25;
    //temp->damage_ = 0.01;
    
    actors.push_back(temp);
  }

  // Light
  mSceneMgr->setAmbientLight(ColourValue(0.4, 0.4, 0.4));
  Light *light = mSceneMgr->createLight("Light1");
  light->setType(Light::LT_DIRECTIONAL);
  light->setDirection(Vector3(-1, -1, -1));
  light->setPosition(Vector3(0, 25000, 25000));
  light->setDiffuseColour(0.3, 0.3, 0.3);
  light->setSpecularColour(0.3, 0.3, 0.3);
    

  //SKYBOX
  mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 5000, false);
  //////////////************
  CEGUI::Window* guiSheet = Locator::getGuiSystem()->getGUISheet();

  CEGUI::Window* text_ = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "HealthText");
  text_->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim(0.02f)));
  text_->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim(0.04f)));
  text_->setText("Score: 0");
  guiSheet->addChildWindow(text_);

  CEGUI::ProgressBar* bar_ = (CEGUI::ProgressBar*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/ProgressBar",
                                                                                "HealthBar");
  bar_->setPosition(CEGUI::UVector2(cegui_reldim(0.01f), cegui_reldim(0.01f)));
  bar_->setSize(CEGUI::UVector2(cegui_reldim(0.107f), cegui_reldim(0.02f)));
  guiSheet->addChildWindow(bar_);
}


////////////////////////////////////////////////////////////////////////////////

bool SideScroller::isDone() {
  return isDone_;
}

////////////////////////////////////////////////////////////////////////////////

GameState* SideScroller::update(const Ogre::Real& timeSinceLastFrame) {
  Ogre::Root* root_ = getRoot();
  Camera* mCamera = root_->getSceneManager("Default SceneManager")->getCamera("MyCamera");
  InputSystem* is = Locator::getInput();
		
	//update AI
	//AIManager(actors, root_);

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
      CEGUI::Window* text_ = CEGUI::WindowManager::getSingleton().getWindow("HealthText");
      static char buf[255];
      sprintf(buf, "Score: %d", player->getScore());
      text_->setText(buf);
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

  CEGUI::ProgressBar* bar_ = (CEGUI::ProgressBar*)CEGUI::WindowManager::getSingleton().getWindow("HealthBar");
  bar_->setProgress(player->getHealth() / player->getMaxHealth());
	
  return NULL;
}

