////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   The main menu of the game
////////////////////////////////////////////////////////////////////////////////
#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

#include "GameState.h"
#include "GuiListener.h"
#include "SideScroller.h"
#include "audio/audio.h"
#include <iostream>
#include <CEGUI/elements/CEGUIPushButton.h>

class MainMenu : public GameState, public GuiListener {
public:
  MainMenu();
  ~MainMenu();
  void initialize();
  GameState* update(const Ogre::Real& timeSinceLastFrame);
  bool isDone();


  bool onStart(const CEGUI::EventArgs& e) {
    titleOverlay->hide();
    LittleManOverlay->show();
    isDone_ = true;
    next_ = new SideScroller();
	
    return true;
  }

  bool onAbout(const CEGUI::EventArgs& e) {
    titleOverlay->hide();
    CEGUI::WindowManager& wm = CEGUI::WindowManager::getSingleton();
    CEGUI::PushButton* aboutmenu = (CEGUI::PushButton*)wm.getWindow("About_Menu");
    CEGUI::PushButton* ok = (CEGUI::PushButton*)wm.getWindow("Ok");
    CEGUI::PushButton* start = (CEGUI::PushButton*)wm.getWindow("Start");
    CEGUI::PushButton* about = (CEGUI::PushButton*)wm.getWindow("About");
    CEGUI::PushButton* credits = (CEGUI::PushButton*)wm.getWindow("Credits");
    CEGUI::PushButton* quit = (CEGUI::PushButton*)wm.getWindow("Quit");

    aboutmenu->show();
    ok->show();
    start->hide();
    about->hide();
    quit->hide();
    credits->show();
    return true;
  }

  bool onOk(const CEGUI::EventArgs& e) {
    titleOverlay->show();
    CEGUI::WindowManager& wm = CEGUI::WindowManager::getSingleton();
    CEGUI::PushButton* ok = (CEGUI::PushButton*)wm.getWindow("Ok");
    CEGUI::PushButton* aboutmenu = (CEGUI::PushButton*)wm.getWindow("About_Menu");
    CEGUI::PushButton* start = (CEGUI::PushButton*)wm.getWindow("Start");
    CEGUI::PushButton* about = (CEGUI::PushButton*)wm.getWindow("About");
    CEGUI::PushButton* credits = (CEGUI::PushButton*)wm.getWindow("Credits");
    CEGUI::PushButton* quit = (CEGUI::PushButton*)wm.getWindow("Quit");

    credits->hide();
    aboutmenu->hide();
    ok->hide();
    start->show();
    about->show();
    quit->show();
    return true;
  }

  bool onCredits(const CEGUI::EventArgs& e) {
    CEGUI::WindowManager& wm = CEGUI::WindowManager::getSingleton();
    CEGUI::PushButton* ok = (CEGUI::PushButton*)wm.getWindow("Ok");
    CEGUI::PushButton* ok2 = (CEGUI::PushButton*)wm.getWindow("Ok2");
    CEGUI::PushButton* aboutmenu = (CEGUI::PushButton*)wm.getWindow("About_Menu");
    CEGUI::PushButton* creditmenu = (CEGUI::PushButton*)wm.getWindow("Credit_Menu");
    CEGUI::PushButton* credits = (CEGUI::PushButton*)wm.getWindow("Credits");

    ok->hide();
    ok2->show();
    aboutmenu->hide();
    credits->hide();
    creditmenu->show();
    return true;
  }
  
  bool onOk2(const CEGUI::EventArgs& e) {
   titleOverlay->show();
   CEGUI::WindowManager& wm = CEGUI::WindowManager::getSingleton();
   CEGUI::PushButton* ok2 = (CEGUI::PushButton*)wm.getWindow("Ok2");
   CEGUI::PushButton* creditmenu = (CEGUI::PushButton*)wm.getWindow("Credit_Menu");
   CEGUI::PushButton* start = (CEGUI::PushButton*)wm.getWindow("Start");
   CEGUI::PushButton* about = (CEGUI::PushButton*)wm.getWindow("About");
   CEGUI::PushButton* quit = (CEGUI::PushButton*)wm.getWindow("Quit");

   start->show();
   about->show();
   quit->show();  

   creditmenu->hide();   
   ok2->hide();
   return true;
  }

 bool onQuit(const CEGUI::EventArgs& e) {
    isDone_ = true;
    return true;
  }





private:
  CEGUI::PushButton* makeButton(const std::string& resourcePath,
                                const std::string& buttonName);
  bool isDone_;
  GameState* next_;
  audBackground* menuMusic_;

   //LittleMan OVERLAY
  Ogre::Overlay* LittleManOverlay;

   //Title OVERLAY
  Ogre::Overlay* titleOverlay;
};

#endif // MAIN_MENU_H_
