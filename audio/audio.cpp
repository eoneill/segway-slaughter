#include <stdlib.h>
#include <dirent.h>
#include <algorithm>
#include "audio.h"

using namespace std;

audBackground::audBackground(int buffersize) {
	alutGetError();				//Clear alut error status
	alGetError();				//Clear al error status

	alutGetMIMETypes(ALUT_LOADER_BUFFER);

	//Check for errors
	ALenum error = alutGetError();

	if (error == AL_INVALID_OPERATION) {
		cout << "alut not initalized before class, initalizing alut to alutInit(NULL, NULL)\n";
		cout << "\tplease initalize alut first by calling alutInit(int *argc, char **argv) in main" << endl;
		alutInit(NULL,NULL);
	}
	if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR) {
		cout << "alutInit error " << alutGetErrorString(error) << endl;
		exit(0);
	}

	alGenSources(1,&backgroundSource);	//Create background source

	//Check for more errors
	error = alGetError();

	if (error != AL_NO_ERROR) {
		cout << "audBackground::alGenSources :" << error << endl;
		exit(0);
	}

	//Set background parameters
	alSource3f(backgroundSource, AL_POSITION,        0.0, 0.0, 0.0);
	alSource3f(backgroundSource, AL_VELOCITY,        0.0, 0.0, 0.0);
	alSource3f(backgroundSource, AL_DIRECTION,       0.0, 0.0, 0.0);
	alSourcef (backgroundSource, AL_ROLLOFF_FACTOR,  0.0          );
	alSourcei (backgroundSource, AL_SOURCE_RELATIVE, AL_TRUE      );
	
	this->buffersize = buffersize;
}

audBackground::~audBackground() {
	alSourcei (backgroundSource, AL_LOOPING,	 AL_FALSE      );
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
			cout << "Background create buffer error: " << alutGetErrorString(error) << " Removing " << files[i] << " from files" << endl;
			files.erase(files.begin()+i);	//Remove problem file from queue
			i--;
			continue;
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
	
	if (files.size() == 1) {
	     alSourcei (backgroundSource, AL_LOOPING, AL_TRUE);
	}
}

int audBackground::audChangeDir(string folder, string type) {
	alSourcei (backgroundSource, AL_LOOPING,	 AL_FALSE      );
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

	if(files.size() == 1) {
		cout << "Only one file loaded, there is no queue to update" << endl;
		return -1;
	}

	for(int i = processed; i > 0; i--) {
	     if(files.empty()) {
	          cout << "Something happend to the files loaded from the dir" << endl;
	          return -1;
	     }
	     
		if(files.size() <= currentFile) {
			currentFile = 0;
		}

		ALuint buffer;

		alSourceUnqueueBuffers(backgroundSource,1,&buffer);
		alDeleteBuffers(1,&buffer);	//Remove old buffer

		alutGetError();		//Clear alut errors
		alGetError();		//Clear al errors

		// and load new one
		buffer = alutCreateBufferFromFile ((currentFolder + "/" + files[currentFile]).c_str());
		//Check for buffer errors
		if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR) {
			cout << "Background create buffer error: " << alutGetErrorString(error) << endl;
			files.erase(files.begin()+currentFile);	//Remove problem file from queue
			i++;
		     continue;
		}

		alSourceQueueBuffers(backgroundSource,1,&buffer); //Queue new buffer
		//Check for queuing errors
		if ((error = alGetError()) != AL_NO_ERROR) {
			cout << "Background buffer queue error: " << error << " Removing " << files[i] << " from queue" << endl;
			alDeleteBuffers(1,&buffer);
			files.erase(files.begin()+currentFile);
			i++;
		}
          else {
		     currentFile++;
		}
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

audSFX::audSFX() {
	alutGetError();				//Clear alut error status
	alGetError();				//Clear al error status

	alutGetMIMETypes(ALUT_LOADER_BUFFER);

	//Check for errors
	ALenum error = alutGetError();

	if (error == AL_INVALID_OPERATION) {
		cout << "alut not initalized before class, initalizing alut to alutInit(NULL, NULL)\n";
		cout << "\tplease initalize alut first by calling alutInit(int *argc, char **argv) in main" << endl;
		alutInit(NULL,NULL);
	}
	if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR) {
		cout << "alutInit error " << alutGetErrorString(error) << endl;
		exit(0);
	}
}

audSFX::~audSFX() {
	alSourceStopv(files.size(),sfxSource);	//Stop playback of sources

	alDeleteSources(files.size(), sfxSource);	//Delete sources
	alDeleteBuffers(files.size(), sfxBuffer);	//Delete buffers

	delete [] sfxSource;
	delete [] sfxBuffer;
}

int audSFX::audGetFilesOfType (string folder, string type) {	
	int typelength = type.length() + 1;
	
	if (type != "") {
		type = "." + type;
	}

	DIR *dir = opendir(folder.c_str());

	if(dir == NULL) {
		cout << "Error opening SFX directory" << endl;
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

int audSFX::audLoadDir(string folder, string type) {
	if (audGetFilesOfType(folder,type) == -1) {
		return -1;
	}

	if (files.empty()) {
		cout << "No audio files in target for SFX" << endl;
		return -1;
	}

	sort(files.begin(),files.end());	//Sort files

	ALenum error;

	alutGetError();		//Clear alut error status
	alGetError();		//Clear al error status

	sfxSource = new ALuint [files.size()];

	alGenSources(files.size(),sfxSource);	//Create SFX sources

	//Check for more errors
	error = alGetError();

	if (error != AL_NO_ERROR) {
		cout << "audSFX::alGenSources :" << error << endl;
		exit(0);
	}

	sfxBuffer = new ALuint [files.size()];

	for(int i = 0; i < files.size(); i++) {

		*(sfxBuffer + i) = alutCreateBufferFromFile ((folder + "/" + files[i]).c_str());
		//Check for buffer errors
		if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR) {
			cout << "SFX create buffer error: " << alutGetErrorString(error) << " Removing " << files[i] << " from files" << endl;
			files.erase(files.begin()+i);	//Remove problem file from queue
			i--;
			continue;
		}

		alSourcei(*(sfxSource + i),AL_BUFFER,*(sfxBuffer + i));
		//Check for source queue errors
		if ((error = alGetError()) != AL_NO_ERROR) {
			cout << "SFX buffer to source error: " << error << " Removing " << files[i] << " from files" << endl;
			alDeleteBuffers(1,sfxBuffer + i);		//Delete problem buffer to save memeory
			files.erase(files.begin()+i);	//Remove problem file from queue
			i--;
		}

	}

	if (files.empty()) {
		cout << "Loading failed for all files" << endl;
		return -1;
	}
}

int audSFX::audChangeDir(string folder, string type) {
	alSourceStopv(files.size(),sfxSource);	//Stop playback of sources

	alDeleteSources(files.size(), sfxSource);	//Delete sources
	alDeleteBuffers(files.size(), sfxBuffer);	//Delete buffers

	delete[] sfxSource;	//Delete dynamic memory
	delete[] sfxBuffer;

	audLoadDir(folder,type);	//Load new dir
}

int audSFX::audPlay(int index) {
	alSourcePlayv(1,sfxSource + index);

	return 0;
}

int audSFX::audPlay(string file) {
	bool found = false;
	for (int i = 0; i < files.size() && !found; i++) {
		if(files[i] == file) {
			found = true;
			alSourcePlayv(1,sfxSource + i);
			return 0;
		}
	}

	return -1;
}

int audSFX::audStop() {	
	alSourceStopv(files.size(),sfxSource);	//Stop all sources
	
	return 0;
}
