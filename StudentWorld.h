#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include <map>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Socrates;
class Dirt;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:
    int m_level;
    Socrates* player;
    Dirt* testDirt;
    
    std::vector<Actor*> m_actors;
    std::map<std::string, int> m_numActors;
    
    void addDirt();
};

#endif // STUDENTWORLD_H_
