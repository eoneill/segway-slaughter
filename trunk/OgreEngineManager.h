////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
// 
// Time-stamp: <Last modified 2010-01-20 15:08:03 by Eric Scrivner>
//
// Description:
//   The interface to the Ogre3D rendering engine and OIS input system.
////////////////////////////////////////////////////////////////////////////////
#ifndef OGREENGINEMANAGER_H__
#define OGREENGINEMANAGER_H__

#include <memory>
#include <string>
#include <map>

#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

////////////////////////////////////////////////////////////////////////////////
// Definitions
#define ENGINEMANAGER OgreEngineManager::Instance()

////////////////////////////////////////////////////////////////////////////////
// Class: OgreEngineManager
//
// The main interface to the Ogre3D and OIS systems
class OgreEngineManager :
  public WindowEventListener,
  public FrameListener
{
public:
  ~OgreEngineManager();

  //////////////////////////////////////////////////////////////////////////////
  // Function: Instance
  //
  // Returns a new instance of the Ogre3D engine manager.
  static OgreEngineManager& Instance() {
    static OgreEngineManager instance;
    return instance;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Function: Startup
  //
  // Loads the given plugin and configuration file for Ogre3D and writes log
  // messages to the given log file.
  bool Startup(const std::string& pluginFileName,
               const std::string& configFileName,
               const std::string& logFileName);

  void Shutdown();
  void ShutdownInput();

  void StartRenderLoop();
  void StopRenderLoop() { mIsEngineRunning = false; }

  void windowClosed(RenderWindow* rw);
  bool frameStarted(const FrameEvent& evt);

  RenderWindow* GetRenderWindow() const { return mWindow; }
  Root* GetRoot() const { return mRoot.get(); }
  OIS::InputManager* GetInputManager() const { return mInputManager; }
  OIS::Keyboard*     GetKeyboard() const { return mKeyboard; }
  OIS::Mouse*        GetMouse() const { return mMouse; }
protected:
  OgreEngineManager();

  void SetupResources();
  bool Configure();
  void SetupInputDevices();

  std::auto_ptr<Root> mRoot;
  Camera*             mGenericCamera;
  RenderWindow*       mWindow;
  bool                mIsEngineRunning;
  OIS::InputManager*  mInputManager;
  OIS::Mouse*         mMouse;
  OIS::Keyboard*      mKeyboard;
};

#endif // OGREENGINEMANAGER_H__
