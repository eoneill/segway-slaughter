#ifndef AIMANAGER_H
#define AIMANAGER_H

#include <Ogre.h>
#include "Actor.h"
//#include "Enemy.h"

using namespace std;
using namespace Ogre;

double getDist(double pos1[3], double pos2[3]);
void AIManager(vector<Actor*>& actors);
void spawnBehind(vector<Actor*>& actors, int & NumEnemies);

#endif
