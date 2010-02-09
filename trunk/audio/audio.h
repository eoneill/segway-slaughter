////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Description:
//   OpenAL audio interfaces
////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_H_
#define AUDIO_H_

// OpenAL includes
#include <AL/alut.h>

// C++ includes
#include <vector>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// Class: audBackground
//
// Handles the loading and playing of background music
class audBackground {
private:
  ALuint backgroundSource;
  std::string currentFolder;     //Folder where background audio comes from
  std::vector<std::string> files;     //Files to queue for background
  size_t buffersize;          //Size of buffer queue
  size_t currentFile;     //Index of files for current file
  int audGetFilesOfType(std::string folder, std::string type);
public:
  audBackground(int buffersize);
  ~audBackground();     //Cleanup
  int audLoadDir(std::string folder, std::string type = "");
  int audChangeDir(std::string folder, std::string type = "");
  ALint audCheck();
  int audUpdateQueue(int play);
  int audPlay();
  int audStop();
};

////////////////////////////////////////////////////////////////////////////////
// Class: audSFX
//
// Handles the loading and playing of sound effects
class audSFX {
private:
  ALuint *sfxSource;
  ALuint *sfxBuffer;
  std::vector<std::string> files;
  int audGetFilesOfType(std::string folder, std::string type);
public:
  audSFX();
  ~audSFX();
  int audLoadDir(std::string folder, std::string type = "");
  int audChangeDir(std::string folder, std::string type = "");
  int audPlay(int index);
  int audPlay(std::string file);
  int audStop();
};

#endif
