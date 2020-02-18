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
    m_numActors["Dirt"] = 0;
    m_numActors["Spray"] = 0;
    m_numActors["Flame"] = 0;
    m_numActors["Flame Goodie"] = 0;
    player = nullptr;
    test = new FlameGoodie(128, 0, this);
    
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    player = new Socrates(this);
    addDirt();
    m_actors.push_back(test);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    checkCollision();
    callDoSomething();
    
    setGameStatText("Score: 004500 Level: " + to_string(getLevel()) + " Lives: " + to_string(getLives()) + " health: " + to_string(player->getHealth()) + " Sprays: " + to_string(player->numSpray())+" Flames: " + to_string(player->numFlame()));
    
    checkDead();
    if (player->isDead())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = m_actors.begin();
    for (;it != m_actors.end();)
    {
        delete *it;
        it = m_actors.erase(it);
    }
    if (player != nullptr)
        delete player;
}

void StudentWorld::addSpray(double startX, double startY, int dir)
{
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Dirt"]);
    m_actors.insert(it, new Spray(startX, startY, dir, this));
    //m_actors.push_back(new Spray(startX, startY, dir, this));
    m_numActors["Spray"] += 1;
}

void StudentWorld::addFlame(double startX, double startY, int dir)
{
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Dirt"] + m_numActors["Spray"]);
    m_actors.insert(it, new Flame(startX, startY, dir, this));
    m_numActors["Flame"] += 1;
}

std::vector<Actor*> StudentWorld::actorVector() const
{
    return m_actors;
}

int StudentWorld::numActors(string actorType)
{
    return m_numActors[actorType];
}

void StudentWorld::decrementActors(string actorType)
{
    m_numActors[actorType] -= 1;
}

Socrates* StudentWorld::getPlayer() const
{
    return player;
}

void StudentWorld::callDoSomething()
{
    vector<Actor*>::iterator it = m_actors.begin();
    for (;it != m_actors.end(); it++)
    {
        (*it)->doSomething();
    }
    player->doSomething();
}

void StudentWorld::checkCollision()
{
    if (m_actors.size() == 0)
        return;
    vector<Actor*>::iterator it = m_actors.begin();
    for (; it != m_actors.end();)
    {
        if ((*it)->didCollide())
        {
            if ((*it)->isDamageable())
                (*it)->makeDead();
        }
        it++;
    }
}

void StudentWorld::checkDead()
{
    vector<Actor*>::iterator it = m_actors.begin();
    for (;it != m_actors.end();)
    {
        if ((*it)->isDead())
        {
            m_numActors[(*it)->getActorType()] -= 1;
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }
}

void StudentWorld::addDirt()
{
    int startPositionalAngle, startRadius;                  // degrees
    double startX, startY;
    for (int i = 0; i < max(180 - (20*getLevel()), 20); i++)
    {
        startPositionalAngle = randInt(0, 360);
        startRadius = randInt(0, 120);
        startX = (startRadius * cos(startPositionalAngle * (M_PI/180)) + VIEW_RADIUS); // switch to polar
        startY = (startRadius * sin(startPositionalAngle * (M_PI/180)) + VIEW_RADIUS);
        
        m_actors.push_back(new Dirt(startX, startY, this));
        m_numActors["Dirt"] += 1;
    }
}
