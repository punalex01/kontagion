#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Socrates;
class Dirt;
class Spray;
class FlameGoodie;
class Fungus;
class Bacteria;
class Salmonella;

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
    double getDistance(double x1, double y1, double x2, double y2) const;
    bool eatFood(Bacteria* bac);
    bool bacBlocking(Bacteria* bac);
    
    void addSpray(double startX, double startY, int dir);
    void addFlame(double startX, double startY, int dir);
    void addBacteria(double startX, double startY, std::string bacType);         // needs bacteria's actorType
    void addBacteria(double startX, double startY, Bacteria* bac);
    void dropFood(double startX, double startY);
    bool findFood(Bacteria* bac, double &foodX, double &foodY);
    bool trackPlayer(Bacteria* bac, std::string type);
    bool blockByDirt(Bacteria* bac);
    
    void dissolvePit();

private:
    Socrates* player;
    int m_pitsDissolved;
    
    std::vector<Actor*> m_actors;
    std::map<std::string, int> m_numActors;
    
    void checkCollision();
    void checkDead();       // check which actors are dead and removes them
    void callDoSomething(); // calls doSomething() for all actors;
    void addDirt();
    void addPit();
    void addFood();
    void addGoodies();
    std::string displayString();
    bool overlapPitFood(double startX, double startY);
    void getStartPoint(double &startX, double &startY);
    
    
    
};

#endif // STUDENTWORLD_H_
