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
class Spray;
class FlameGoodie;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    std::vector<Actor*> actorVector() const;
    int numActors(std::string actorType); // returns num of specific actor
    void decrementActors(std::string actorType);
    Socrates* getPlayer() const;
    
    void addSpray(double startX, double startY, int dir);
    void addFlame(double startX, double startY, int dir);

private:
    Socrates* player;
    
    std::vector<Actor*> m_actors;
    std::map<std::string, int> m_numActors;
    FlameGoodie* test;
    
    void checkCollision();
    void checkDead();       // check which actors are dead and removes them
    void callDoSomething(); // calls doSomething() for all actors;
    void addDirt();
};

#endif // STUDENTWORLD_H_
