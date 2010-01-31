////////////////////////////////////////////////////////////////////////////////
// Segway Slaughter
//
// Time-stamp: <Last modified 2010-01-30 17:10:17 by Eric Scrivner>
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
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// Class: audBackground
//
// Handles the loading and playing of background music
class audBackground {
     private:
          ALuint backgroundSource;
          string currentFolder;     //Folder where background audio comes from
          vector<string> files;     //Files to queue for background
          int buffersize;          //Size of buffer queue
          int currentFile;     //Index of files for current file
          int audGetFilesOfType(string folder, string type);
     public:
          audBackground(int buffersize);
          ~audBackground();     //Cleanup
          int audLoadDir(string folder, string type = "");
          int audChangeDir(string folder, string type = "");
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
          vector<string> files;
          int audGetFilesOfType(string folder, string type);
     public:
          audSFX();
          ~audSFX();
          int audLoadDir(string folder, string type = "");
          int audChangeDir(string folder, string type = "");
          int audPlay(int index);
          int audPlay(string file);
          int audStop();
};

#endif
