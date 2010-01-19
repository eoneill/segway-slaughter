#ifndef AUDIO
#define AUDIO

#include <AL/alut.h>
#include <vector>
#include <iostream>

using namespace std;

class audBackground {
	private:
		ALuint backgroundSource;
		string currentFolder;	//Folder where background audio comes from
		vector<string> files;	//Files to queue for background
		int buffersize;		//Size of buffer queue
		int currentFile;	//Index of files for current file
		int audGetFilesOfType(string folder, string type);
	public:
		audBackground(int buffersize);
		~audBackground();	//Cleanup
		int audLoadDir(string folder, string type = "");
		int audChangeDir(string folder, string type = "");
		ALint audCheck();
		int audUpdateQueue(int play);
		int audPlay();
		int audStop();
};

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
