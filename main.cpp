#include <Ogre.h>
#include "Actor.h"
#include "Application.h"
#include "Locator.h"

#include <iostream>
using namespace std;
using namespace Ogre;

class ExitListener : public FrameListener, public OIS::KeyListener {
public:
  ExitListener()
    : isRunning(true)
  {
    Locator::getInput()->addKeyListener(this);
  }

  bool frameStarted(const FrameEvent& ev) {
    return isRunning;
  }
  
  bool keyPressed(const OIS::KeyEvent& arg) {
    if (arg.key == OIS::KC_ESCAPE) {
      isRunning = false;
    }

    return true;
  }

  bool keyReleased(const OIS::KeyEvent& arg) {
    return true;
  }
private:
  bool isRunning;
};

class SideScroller : public FrameListener, public OIS::KeyListener {
public:
  SideScroller(Root* root)
    : root_(root)
  {
    Locator::getInput()->addKeyListener(this);
  }

  bool frameStarted(const FrameEvent& ev) {
    SceneNode* node = root_->getSceneManager("Default SceneManager")->getSceneNode(player.sceneNode);
    Camera* mCamera = root_->getSceneManager("Default SceneManager")->getCamera("MyCamera");
		InputSystem* is = Locator::getInput();
		
    if (is->isKeyDown(OIS::KC_UP)) {
      node->translate(Vector3(-1,0,0));
      player.position[0]--;
    }
    if (is->isKeyDown(OIS::KC_DOWN)) {
      node->translate(Vector3(1,0,0));
      player.position[0]++;
    }
    if (is->isKeyDown(OIS::KC_LEFT)) {
      mCamera->move(Vector3(0,0,1));
				
      node->translate(Vector3(0,0,1));
      player.position[2] += 1;

      if(player.facingRight == true){
        node->yaw(Ogre::Degree(180));
        player.facingRight = false;
      }
    }
    if (is->isKeyDown(OIS::KC_RIGHT)) {     				
      mCamera->move(Vector3(0,0,-1));

      node->translate(Vector3(0,0,-1));
      player.position[2] -= 1;
				
      if(player.facingRight != true){
        node->yaw(Ogre::Degree(180));
        player.facingRight = true;
      }
    }
    if (is->isKeyDown(OIS::KC_ESCAPE)) {
      return false;
    }

    return true;
  }

  bool keyPressed(const OIS::KeyEvent& arg) {    
    return true;
  }

  bool keyReleased(const OIS::KeyEvent& arg) {
    return true;
  }

  void initialize() {
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
    player.facingRight = true;
    player.sceneNode = "NinjaNode2";
    Entity *ent2 = mSceneMgr->createEntity( "ninja", "ninja.mesh" );
    SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( player.sceneNode, Vector3( 0, 0, 0 ) );
    node2->attachObject( ent2 );

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
private:
  Root* root_;
  Actor player;
};

class SegwaySlaughter : public Application {
public:
  SegwaySlaughter()
    : Application::Application("Segway Slaughter"),
      sideScroller_(0)
  { }

  ~SegwaySlaughter() {
    if (sideScroller_) {
      delete sideScroller_;
    }
  }

  void initialize() {
    sideScroller_  = new SideScroller(root_);
    sideScroller_->initialize();

    root_->addFrameListener(sideScroller_);
  }
private:
  SideScroller* sideScroller_;
};

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
  int main(int argc, char **argv)
#endif
{
  try
  {
    SegwaySlaughter app;
    app.go();
  }
  catch(Exception& e)
  {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
  }

  return 0;
}
