////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   The main menu of the game
////////////////////////////////////////////////////////////////////////////////
#include "MainMenu.h"
#include "Locator.h"

using namespace Ogre;

MainMenu::MainMenu()
  : isDone_(false), next_(NULL) {
  Locator::getInput()->addMouseListener(this);
}

////////////////////////////////////////////////////////////////////////////////

MainMenu::~MainMenu() {
  SceneManager* mSceneMgr = getRoot()->getSceneManager("Default SceneManager");
  mSceneMgr->destroyAllCameras();
  mSceneMgr->destroyAllEntities();
  mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
  mSceneMgr->destroyAllLights();
  getRoot()->getAutoCreatedWindow()->removeAllViewports();

  //  CEGUI::Window* guiSheet = Locator::getGuiSystem()->getGUISheet();
  CEGUI::WindowManager& wm = CEGUI::WindowManager::getSingleton();
  wm.destroyWindow("Start");
  wm.destroyWindow("Options");
  wm.destroyWindow("Quit");

  Locator::getInput()->removeMouseListener(this);
}

////////////////////////////////////////////////////////////////////////////////

void MainMenu::initialize() {
  CEGUI::Window* guiSheet = Locator::getGuiSystem()->getGUISheet();

  // Start button
  CEGUI::PushButton* startBtn = makeButton("TaharezLook/Button", "Start");
  startBtn->setPosition(CEGUI::UVector2(cegui_reldim(0.47f),
                                        cegui_reldim( 0.3f)));
  startBtn->setSize(CEGUI::UVector2(cegui_reldim(.09f),
                                    cegui_reldim( .06f)) );
  startBtn->setText("Start");
  guiSheet->addChildWindow(startBtn);

  // Options button
  CEGUI::PushButton* optnsBtn = makeButton("TaharezLook/Button", "Options");
  optnsBtn->setPosition(CEGUI::UVector2(cegui_reldim(0.47f),
                                             cegui_reldim( 0.4f)));
  optnsBtn->setSize(CEGUI::UVector2(cegui_reldim(.09f),
                                         cegui_reldim( .06f)));
  optnsBtn->setText("Options");
  guiSheet->addChildWindow(optnsBtn);

  // Quit button
  CEGUI::PushButton* quitBtn  = makeButton("TaharezLook/Button", "Quit");
  quitBtn->setPosition(CEGUI::UVector2(cegui_reldim(0.47f),
                                          cegui_reldim( 0.5f)));
  quitBtn->setSize(CEGUI::UVector2(cegui_reldim(.09f),
                                      cegui_reldim( .06f)));
  quitBtn->setText("Quit");
  guiSheet->addChildWindow(quitBtn); 

  CEGUI::WindowManager* wm = &CEGUI::WindowManager::getSingleton();
  wm->getWindow("Start")->subscribeEvent(CEGUI::PushButton::EventClicked,
                                         CEGUI::Event::Subscriber(&MainMenu::onStart, this));

  wm->getWindow("Quit")->subscribeEvent(CEGUI::PushButton::EventClicked,
                                        CEGUI::Event::Subscriber(&MainMenu::onQuit, this));


  //////////////////////////////////////////////////////////////////////////////

  //Create the scene manager
  SceneManager *mSceneMgr = getRoot()->getSceneManager("Default SceneManager");
  mSceneMgr->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));
  mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
  
  //Setup the camera
  Camera *mCamera = mSceneMgr->createCamera("MyCamera");
  mCamera->setNearClipDistance(5);
  Viewport *mViewport = getRoot()->getAutoCreatedWindow()->addViewport(mCamera);
  
  //			Camera *mCamera = mRoot->getSceneManager("Default SceneManager")->getCamera("MyCamera");
  mCamera->setPosition(0,100,200);
  mCamera->setDirection(0,0,0);
  
  
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
	
  //Robot
  Entity *ent2 = mSceneMgr->createEntity( "Robot2", "robot.mesh" );
  SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode2", Vector3( 50, 0, 0 ) );
  node2->attachObject( ent2 );
  
  // Light
  mSceneMgr->setAmbientLight(ColourValue(0.4, 0.4, 0.4));
  Light *light = mSceneMgr->createLight("Light1");
  light->setType(Light::LT_DIRECTIONAL);
  light->setDirection(Vector3(-1, -1, -1));
  light->setPosition(Vector3(0, 25000, 25000));
  light->setDiffuseColour(0.3, 0.3, 0.3);
  light->setSpecularColour(0.3, 0.3, 0.3);
  
  // FOG
  ColourValue fadeColour(0.2, 0.2, 0.2);
  mViewport->setBackgroundColour(fadeColour);
  mSceneMgr->setFog(FOG_LINEAR, fadeColour, 0.0, 500, 50000);
  
  
  //SKYBOX
  mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 5000, false);
}

////////////////////////////////////////////////////////////////////////////////

GameState* MainMenu::update() {
  return next_;
}

////////////////////////////////////////////////////////////////////////////////

bool MainMenu::isDone() {
  return isDone_;
}

////////////////////////////////////////////////////////////////////////////////

CEGUI::PushButton* MainMenu::makeButton(const std::string& resourcePath,
                                        const std::string& buttonName)
{
  CEGUI::WindowManager* wm = &CEGUI::WindowManager::getSingleton();
  return (CEGUI::PushButton*)wm->createWindow(resourcePath,
                                             (CEGUI::utf8*)buttonName.c_str());
}
