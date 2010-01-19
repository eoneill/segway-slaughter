#include <stdlib.h>
#include <dirent.h>
#include <algorithm>
#include "audio.h"

using namespace std;

audBackground::audBackground(int buffersize) {
	alutGetError();				//Clear alut error status
	alGetError();				//Clear al error status

	alGenSources(1,&backgroundSource);	//Create background source

	//Check for errors
	ALenum error = alGetError();

	if (error == AL_INVALID_OPERATION) {
		cout << "alut not initalized, initalizing alut to alutInit(NULL, NULL)\n";
		cout << "\tplease initalize alut first by calling alutInit(int *argc, char **argv)" << endl;
		alutInit(NULL,NULL);
		if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR) {
			cout << "alutInit error " << alutGetErrorString(error) << endl;
			exit(0);
		}
		alGenSources(1,&backgroundSource);
		error = alGetError();
	}
	if (error != AL_NO_ERROR) {
		cout << "audBackground::alGenSources :" << error << endl;
		exit(0);
	}
	//Finished checking for errors
	
	this->buffersize = buffersize;
}

audBackground::~audBackground() {
	alSourceStop(backgroundSource);		//Stop playback

	ALint qcount;				
	alGetSourcei(backgroundSource,AL_BUFFERS_QUEUED,&qcount); //Count the queue

	ALuint buffer[qcount];			//Get buffer

	alSourceUnqueueBuffers(backgroundSource,qcount,buffer);	//Remove buffers from queue
	alDeleteBuffers(qcount, buffer);			//Delete buffers
	alDeleteSources(1, &backgroundSource);			//Delete sources			
}

int audBackground::audGetFilesOfType (string folder, string type) {	
	int typelength = type.length() + 1;
	
	if (type != "") {
		type = "." + type;
	}

	DIR *dir = opendir(folder.c_str());

	if(dir == NULL) {
		cout << "Error opening background directory" << endl;
		return -1;
	}

	struct dirent *cfile;

	//Sift through files types
	while((cfile = readdir(dir)) != NULL) {
		files.push_back(cfile->d_name);
		int n = (files.back()).length();
		if (n < typelength && type != "") {
			files.pop_back();
		}
		else if (files.back() == ".." || files.back() == ".") {
			files.pop_back();
		}
		else if ((files.back()).substr(n-typelength) != type && type != "") {
				files.pop_back();
		}
	}

	return 0;
}

int audBackground::audLoadDir(string folder, string type) {
	currentFolder = folder;
	if (audGetFilesOfType(folder,type) == -1) {
		return -1;
	}

	if (files.empty()) {
		cout << "No audio files in target for background" << endl;
		return -1;
	}

	currentFile = 0;
	ALuint backgroundBuffer;
	ALenum error;

	alutGetError();		//Clear alut error status
	alGetError();		//Clear al error status

	for(int i = 0; i < buffersize && i < files.size(); i++) {

		backgroundBuffer = alutCreateBufferFromFile ((folder + "/" + files[i]).c_str());
		//Check for buffer errors
		if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR) {
			cout << "Background create buffer error: " << alutGetErrorString(error) << endl;
			return -1;
		}

		alSourceQueueBuffers(backgroundSource,1,&backgroundBuffer);
		//Check for source queue errors
		if ((error = alGetError()) != AL_NO_ERROR) {
			cout << "Background buffer queue error: " << error << " Removing " << files[i] << " from queue" << endl;
			alDeleteBuffers(1,&backgroundBuffer);	//Delete problem buffer to save memeory
			files.erase(files.begin()+currentFile);	//Remove problem file from queue
			i--;
		}
		else {
			currentFile++;
		}

	}

	if (files.empty()) {
		cout << "Queueing failed for all files" << endl;
		return -1;
	}
}

int audBackground::audChangeDir(string folder, string type) {
	alSourceStop(backgroundSource);	//Stop playback

	ALint qcount;
	alGetSourcei(backgroundSource,AL_BUFFERS_QUEUED,&qcount);	//count the queue

	ALuint buffer[qcount];		//Store retreaved buffers

	alSourceUnqueueBuffers(backgroundSource,qcount,buffer);	//Remove buffers from queue
	alDeleteBuffers(qcount, buffer);			//Delete buffers

	audLoadDir(folder,type);	//Load new dir
}

int audBackground::audUpdateQueue(int play) {
	ALint processed;
	ALenum error;

	alGetSourcei(backgroundSource, AL_BUFFERS_PROCESSED, &processed);

	for(int i = processed; i > 0; i--) {
		if(files.size() <= currentFile) {
			currentFile = 0;
		}

		ALuint buffer;

		alSourceUnqueueBuffers(backgroundSource,1,&buffer);

		alutGetError();		//Clear alut errors
		alGetError();		//Clear al errors

		alDeleteBuffers(1,&buffer);	//Remove old buffer and load new one
		buffer = alutCreateBufferFromFile ((currentFolder + "/" + files[currentFile]).c_str());
		//Check for buffer errors
		if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR) {
			cout << "Background create buffer error: " << alutGetErrorString(error) << endl;
			return -1;
		}

		alSourceQueueBuffers(backgroundSource,1,&buffer); //Queue new buffer
		//Check for queuing errors
		if ((error = alGetError()) != AL_NO_ERROR) {
			cout << "Background buffer queue error: " << error << " Removing " << files[i] << " from queue" << endl;
			alDeleteBuffers(1,&buffer);
			files.erase(files.begin()+currentFile);
			i++;
		}

		currentFile++;
	}

	if(audCheck() != AL_PLAYING && play) { //Start playing if stoped
		audPlay();
	}

	return 0;
}

ALint audBackground::audCheck() {
	ALint value;

	alGetSourcei(backgroundSource, AL_SOURCE_STATE, &value); //Get state of source

	return value;
}

int audBackground::audPlay() {
	if(audCheck() != AL_PLAYING) { //Start playing if stoped
		alSourcePlay(backgroundSource);
		return 0;
	}

	return -1;
}

int audBackground::audStop() {	
	if(audCheck() != AL_STOPPED) { //Stop if playing
		alSourceStop(backgroundSource);
		return 0;
	}
	
	return -1;
}


