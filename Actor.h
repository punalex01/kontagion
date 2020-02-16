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
    virtual bool isDead() const = 0;                  // check if dead
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const;
    
    //virtual bool checkOverlap() = 0;
private:
    StudentWorld* m_world;              // might move to just Socrates
};

class Living : public Actor
{
public:
    Living(int imageID, double startX, double startY, Direction dir, int depth, double size, int health, StudentWorld* world);
    virtual ~Living();
    virtual bool isDead() const;                  // check if dead
    int getHealth() const;
    void addHealth(int health);
    
private:
    int m_health;
};

class NonLiving : public Actor
{
public:
    NonLiving(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* world);
    virtual ~NonLiving();
    virtual bool isDead() const;
    void makeDead();          // for collision
    
private:
    bool m_alive;
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
    Dirt(double startX, double startY, StudentWorld* world);
    virtual ~Dirt();
    virtual void doSomething();
    
private:
};


#endif // ACTOR_H_
