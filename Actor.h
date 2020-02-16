#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const;
private:
    StudentWorld* m_world;              // might move to just Socrates
};

class Living : public Actor
{
public:
    Living(int imageID, double startX, double startY, Direction dir, int depth, double size, int health, StudentWorld* world);
    virtual ~Living();
    bool isDead() const;                  // check if dead
    int getHealth() const;
    void addHealth(int health);
    
private:
    int m_health;
};

class NonLiving : public Actor
{
public:
    NonLiving(int imageID, double startX, double startY, Direction dir, int depth, double size);
    virtual ~NonLiving();
};

class Socrates : public Living
{
public:
    Socrates(StudentWorld* world);
    virtual ~Socrates();
    virtual void doSomething();
    
private:
    double m_positionalAngle;

    void moveSocrates(std::string direction);
};

class Dirt : public NonLiving
{
public:
    Dirt();
};


#endif // ACTOR_H_
