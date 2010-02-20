#include "CasinoLevel.h"
#include "MainMenu.h"

using namespace std;
using namespace Ogre;


CasinoLevel::CasinoLevel()
  : isDone_(false)
{}

////////////////////////////////////////////////////////////////////////////////

CasinoLevel::~CasinoLevel() {
  delete player;
  for(unsigned int i = 0; i < enemies.size(); i++) {
  	delete enemies[i];
  }
  SceneManager* mSceneMgr = getRoot()->getSceneManager("Default SceneManager");
  mSceneMgr->destroyAllCameras();
  mSceneMgr->destroyAllEntities();
  mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
  mSceneMgr->destroyAllLights();
  getRoot()->getAutoCreatedWindow()->removeAllViewports();
}

///////////////////////////////////////////////////////////////////////////////

void CasinoLevel::initialize() {
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
  player = new Actor(root_,"ninja","robot.mesh");
  
  //make some sample enemies
   srand ( time(NULL) );
   for(int i = 0; i < 100; i++){
    char EntName[40] = "Robot";
    sprintf(EntName,"robot%d",i);
    Actor* temp = new Actor(root_,EntName,"ninja.mesh",rand() % LEVEL_WIDTH - LEVEL_WIDTH/2,0,-(rand() % 30000+2000));
    enemies.push_back(temp);
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
}


////////////////////////////////////////////////////////////////////////////////

bool CasinoLevel::isDone() {
  return isDone_;
}

////////////////////////////////////////////////////////////////////////////////

GameState* CasinoLevel::update(const Ogre::Real& timeSinceLastFrame) {
  Ogre::Root* root_ = getRoot();
  Camera* mCamera = root_->getSceneManager("Default SceneManager")->getCamera("MyCamera");
  InputSystem* is = Locator::getInput();
		
  //Move player up, but with constraints
  if (is->isKeyDown(OIS::KC_UP)) {
 		player->move(DI_UP, enemies);
  }
  //Move player down, but with constraints
  if (is->isKeyDown(OIS::KC_DOWN)) {
  	player->move(DI_DOWN, enemies);
  }
  //move player left
    if (is->isKeyDown(OIS::KC_LEFT)) {
	    if(player->move(DI_LEFT, enemies))
	    	{
		      mCamera->move(Vector3(0,0,1.33));
		    }
    }
    //move player right
    if (is->isKeyDown(OIS::KC_RIGHT)) {
    	if(player->move(DI_RIGHT, enemies))
    	{
	      mCamera->move(Vector3(0,0,-1.33));
      }
    }

    if (is->isKeyDown(OIS::KC_A)) {
      player->attack(enemies, root_);
    }
    else{
      player->stopBlood();
      for(unsigned int i = 0; i < enemies.size(); i++)
        enemies[i]->stopBlood();
    }

  if (is->isKeyDown(OIS::KC_ESCAPE)) {
    isDone_ = true;
    //return new MainMenu;
  }
	
  return NULL;
}

