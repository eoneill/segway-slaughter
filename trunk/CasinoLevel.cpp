#include "CasinoLevel.h"

CasinoLevel CasinoLevel::CasinoLevel_;

bool CasinoLevel::update(Application* app) {
  SceneNode* node = root_->getSceneManager("Default SceneManager")->getSceneNode(player.sceneNode);
  Camera* mCamera = root_->getSceneManager("Default SceneManager")->getCamera("MyCamera");
  InputSystem* is = Locator::getInput();
		
  //Move player up, but with constraints
  if (is->isKeyDown(OIS::KC_UP)) {
    	if(player.position[0] > -LEVEL_WIDTH/2){
	      node->translate(Vector3(-1,0,0));
  	    player.position[0]-=player.speed;
  	  }
  }
  //Move player down, but with constraints
  if (is->isKeyDown(OIS::KC_DOWN)) {
    if(player.position[0] < LEVEL_WIDTH/2){
      node->translate(Vector3(1,0,0));
      player.position[0]+=player.speed;
    }
  }
  //move player left
    if (is->isKeyDown(OIS::KC_LEFT)) {
      mCamera->move(Vector3(0,0,1));
				
      node->translate(Vector3(0,0,1));
      player.position[2]+=player.speed;

      if(player.FacingRight == true){
				node->yaw(Ogre::Degree(180));
				player.FacingRight = false;
      }
    }
    //move player right
    if (is->isKeyDown(OIS::KC_RIGHT)) {     				
      mCamera->move(Vector3(0,0,-1));

      node->translate(Vector3(0,0,-1));
      player.position[2]-=player.speed;
				
      if(player.FacingRight != true){
				node->yaw(Ogre::Degree(180));
        player.FacingRight = true;
      }
    }
  if (is->isKeyDown(OIS::KC_ESCAPE)) {
    return false;
  }

  return true;
}

void CasinoLevel::initialize(Root* root) {
  root_ = root;
  
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
  player.position[0] = 0;
  player.position[1] = 0;
  player.position[2] = 0;
  player.speed = 1;
  player.FacingRight = true;
  player.MaxHealth = 100;
  player.CurrentHealth = 100;
  player.sceneNode = "NinjaNode2";
  Entity *ent2 = mSceneMgr->createEntity( "ninja", "robot.mesh" );
  SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( player.sceneNode, Vector3( 0, 0, 0 ) );
  node2->attachObject( ent2 );
  
  //make some sample enemies
  vector <Enemy> enemies;
  srand ( time(NULL) );
  for(int i = 0; i < 25; i++){
    Enemy temp;
    temp.position[0] = rand() % LEVEL_WIDTH - LEVEL_WIDTH/2;
    temp.position[1] = 0;
    temp.position[2] = rand() % 10000 - 5000;;
    temp.FacingRight = true;
    temp.MaxHealth = 100;
    temp.CurrentHealth = 100;
    char NodeNum[40] = "Enemy Node";
    sprintf(NodeNum,"EnemyNode%d",i);
    temp.sceneNode = NodeNum;
    char EntName[40] = "Robot";
    sprintf(EntName,"robot%d",i);
    Entity *ent3 = mSceneMgr->createEntity( EntName, "ninja.mesh" );
    SceneNode *node3 = mSceneMgr->getRootSceneNode()->createChildSceneNode( temp.sceneNode, Vector3( temp.position[0], temp.position[1], temp.position[2] ) );
    node3->attachObject( ent3 );
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

void CasinoLevel::suspend() {}

void CasinoLevel::resume() {}

void CasinoLevel::clean() {
  SceneManager* mSceneMgr = root_->getSceneManager("Default SceneManager");
  mSceneMgr->destroyAllCameras();
  mSceneMgr->destroyAllEntities();
  mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
  mSceneMgr->destroyAllLights();
  root_->getAutoCreatedWindow()->removeAllViewports();
  cerr << "CasinoLevel cleaned" << endl;
  }
