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
  delete menuMusic_;

  SceneManager* mSceneMgr = getRoot()->getSceneManager("Default SceneManager");
  mSceneMgr->setSkyBox(false, "Examples/SpaceSkyBox");
  mSceneMgr->destroyAllCameras();
  mSceneMgr->destroyAllEntities();
  mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
  mSceneMgr->destroyAllLights();
  getRoot()->getAutoCreatedWindow()->removeAllViewports();

  //  CEGUI::Window* guiSheet = Locator::getGuiSystem()->getGUISheet();
  CEGUI::WindowManager& wm = CEGUI::WindowManager::getSingleton();
  wm.destroyWindow("Start");
  wm.destroyWindow("About");
  wm.destroyWindow("About_Menu");
  wm.destroyWindow("Ok");
  wm.destroyWindow("Ok2");
  wm.destroyWindow("Credits");
  wm.destroyWindow("Credit_Menu");
  wm.destroyWindow("Quit");

  Locator::getInput()->removeMouseListener(this);
}

////////////////////////////////////////////////////////////////////////////////

void MainMenu::initialize() {
  CEGUI::Window* guiSheet = Locator::getGuiSystem()->getGUISheet();

   // Start button
  CEGUI::PushButton* startBtn = makeButton("TaharezLook/Button", "Start");
  startBtn->setPosition(CEGUI::UVector2(cegui_reldim(0.47f),
                                        cegui_reldim( 0.5f)));
  startBtn->setSize(CEGUI::UVector2(cegui_reldim(.09f),
                                    cegui_reldim( .06f)) );
  startBtn->setText("Start");
  guiSheet->addChildWindow(startBtn);

  // About button
  CEGUI::PushButton* aboutBtn = makeButton("TaharezLook/Button", "About");
  aboutBtn->setPosition(CEGUI::UVector2(cegui_reldim(0.47f),
                                             cegui_reldim( 0.6f)));
  aboutBtn->setSize(CEGUI::UVector2(cegui_reldim(.09f),
                                         cegui_reldim( .06f)));
  aboutBtn->setText("About");
  guiSheet->addChildWindow(aboutBtn);

  // AboutMenu button
  CEGUI::PushButton* aboutmenu;
  aboutmenu = makeButton("TaharezLook/Button", "About_Menu");
  aboutmenu->setPosition(CEGUI::UVector2(cegui_reldim(0.27f),
                                             cegui_reldim( 0.1f)));
  aboutmenu->setSize(CEGUI::UVector2(cegui_reldim(.5f),
                                         cegui_reldim( .5f)));
  aboutmenu->setText("Premise:\nYou must defeat Don Johnson....\n \nControls:\nPress 'A' to kill.\nUse Arrow Keys to move.");
  guiSheet->addChildWindow(aboutmenu);
  aboutmenu->hide();

  // Ok button
  CEGUI::PushButton* ok;
  ok = makeButton("TaharezLook/Button", "Ok");
  ok->setPosition(CEGUI::UVector2(cegui_reldim(0.36f),
                                             cegui_reldim( 0.45f)));
  ok->setSize(CEGUI::UVector2(cegui_reldim(.06f),
                                         cegui_reldim( .05f)));
  ok->setText("Ok");
  ok->setAlwaysOnTop(true);
  guiSheet->addChildWindow(ok);
  ok->hide();

  // Credits button
  CEGUI::PushButton* credits;
  credits = makeButton("TaharezLook/Button", "Credits");
  credits->setPosition(CEGUI::UVector2(cegui_reldim(0.62f),
                                             cegui_reldim( 0.45f)));
  credits->setSize(CEGUI::UVector2(cegui_reldim(.08f),
                                         cegui_reldim( .05f)));
  credits->setText("Credits");
  credits->setAlwaysOnTop(true);
  guiSheet->addChildWindow(credits);
  credits->hide();

   // CreditsMenu button
  CEGUI::PushButton* creditmenu;
  creditmenu = makeButton("TaharezLook/Button", "Credit_Menu");
  creditmenu->setPosition(CEGUI::UVector2(cegui_reldim(0.27f),
                                             cegui_reldim( 0.06f)));
  creditmenu->setSize(CEGUI::UVector2(cegui_reldim(.5f),
                                         cegui_reldim( .7f)));
  creditmenu->setText("Credits:\n \nGraphics:\n...\n \nAudio:\n...\n \nMisc:...");
  guiSheet->addChildWindow(creditmenu);
  creditmenu->hide();

  // Ok2 button
  CEGUI::PushButton* ok2;
  ok2 = makeButton("TaharezLook/Button", "Ok2");
  ok2->setPosition(CEGUI::UVector2(cegui_reldim(0.49f),
                                          cegui_reldim( 0.6f)));
  ok2->setSize(CEGUI::UVector2(cegui_reldim(.06f),
                                         cegui_reldim( .05f)));
  ok2->setText("Ok");
  ok2->setAlwaysOnTop(true);
  guiSheet->addChildWindow(ok2);
  ok2->hide();


  // Quit button
  CEGUI::PushButton* quitBtn  = makeButton("TaharezLook/Button", "Quit");
  quitBtn->setPosition(CEGUI::UVector2(cegui_reldim(0.47f),
                                          cegui_reldim( 0.7f)));
  quitBtn->setSize(CEGUI::UVector2(cegui_reldim(.09f),
                                      cegui_reldim( .06f)));
  quitBtn->setText("Quit");
  guiSheet->addChildWindow(quitBtn); 

  CEGUI::WindowManager* wm = &CEGUI::WindowManager::getSingleton();
  wm->getWindow("Start")->subscribeEvent(CEGUI::PushButton::EventClicked,
                                         CEGUI::Event::Subscriber(&MainMenu::onStart, this));

  wm->getWindow("About")->subscribeEvent(CEGUI::PushButton::EventClicked,
                                         CEGUI::Event::Subscriber(&MainMenu::onAbout, this));

  wm->getWindow("Quit")->subscribeEvent(CEGUI::PushButton::EventClicked,
                                        CEGUI::Event::Subscriber(&MainMenu::onQuit, this));

  wm->getWindow("Ok")->subscribeEvent(CEGUI::PushButton::EventClicked,
                                        CEGUI::Event::Subscriber(&MainMenu::onOk, this));
 
  wm->getWindow("Credits")->subscribeEvent(CEGUI::PushButton::EventClicked,
                                        CEGUI::Event::Subscriber(&MainMenu::onCredits, this));

  wm->getWindow("Ok2")->subscribeEvent(CEGUI::PushButton::EventClicked,
                                        CEGUI::Event::Subscriber(&MainMenu::onOk2, this));


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
	

  //Mobster
  Entity *ent2 = mSceneMgr->createEntity( "Mobster", "mobster.mesh" );
  SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "mobster", Vector3( 50, -0, -0 ) );
  node2->attachObject( ent2 );
  node2->yaw( Degree( 25 ) );
  node2->setScale( 12,12,12 );

  //Charlie
  Entity *ent3 = mSceneMgr->createEntity( "Charlie", "charlie.mesh" );
  SceneNode *node3 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "charlie", Vector3( -50, 0, 0 ) );
  node3->attachObject( ent3 );
  node3->yaw( Degree( -25 ) );
  node3->setScale( 12,12,12 );

  
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
  mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

 
  //SegwaySlaughter Title OVERLAY
  titleOverlay = OverlayManager::getSingleton().getByName("Segway/Overlay");
  titleOverlay->show();

  //LittleMan OVERLAY
  LittleManOverlay = OverlayManager::getSingleton().getByName("LittleMan/Overlay");
  LittleManOverlay->hide();

  //AUDIO
  menuMusic_ = new audBackground(1);
  menuMusic_->audLoadDir("resources/audio/music/Main_Menu","wav");
  menuMusic_->audPlay();

}

////////////////////////////////////////////////////////////////////////////////

GameState* MainMenu::update(const Ogre::Real& timeSinceLastFrame) {
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
