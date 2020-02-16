#include "StudentWorld.h"
#include "GameConstants.h"

#include "Actor.h"

#include <string>
#include <iterator>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_level = 1;
}

int StudentWorld::init()
{
    player = new Socrates(this);
    addDirt();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if (player->isDead())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    player->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}

void StudentWorld::addDirt()
{
    int startPositionalAngle, startRadius;                  // degrees
    double startX, startY;
    for (int i = 0; i < max(180 - (20*m_level), 20); i++)
    {
        startPositionalAngle = randInt(0, 360);
        startRadius = randInt(0, 120);
        startX = (startRadius * cos(startPositionalAngle * (M_PI/180)) + 128);
        startY = (startRadius * sin(startPositionalAngle * (M_PI/180)) + 128);
        
        m_actors.push_back(new Dirt(startX, startY, this));
    }
}
