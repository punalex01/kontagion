#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, depth, size)
{
    m_world = world;
}

Actor::~Actor()
{
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

Living::Living(int imageID, double startX, double startY, Direction dir, int depth, double size, int health, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, size, world)
{
    m_health = health;
}

Living::~Living()
{
}

bool Living::isDead() const
{
    return m_health == 0;
}

int Living::getHealth() const
{
    return m_health;
}

void Living::addHealth(int health)
{
    m_health += health;
}

NonLiving::NonLiving(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, size, world)
{
    m_alive = true;
}

NonLiving::~NonLiving()
{
}

bool NonLiving::isDead() const
{
    return !m_alive;
}

Socrates::Socrates(StudentWorld* world)
: Living(IID_PLAYER, 0, 128, 0, 0, 1, 100, world)
{
    m_positionalAngle = M_PI;
}

Socrates::~Socrates()
{
}

void Socrates::doSomething()
{
    if (isDead())
        return;
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                moveSocrates("left");
                break;
            case KEY_PRESS_RIGHT:
                moveSocrates("right");
                break;
        }
    }
}

void Socrates::moveSocrates(string direction)
{
    if (direction == "left")                // pressed left key
    {
        setDirection(getDirection()+5);
        m_positionalAngle += 5 * (M_PI / 180);
        moveTo(128*(cos(m_positionalAngle))+128, 128*(sin(m_positionalAngle))+128);
    }
    else                                    // pressed right key
    {
        setDirection(getDirection()-5);
        m_positionalAngle -= 5 * (M_PI / 180);
        moveTo(128*(cos(m_positionalAngle))+128, 128*(sin(m_positionalAngle))+128);
    }
}

Dirt::Dirt(double startX, double startY, StudentWorld* world)
: NonLiving(IID_DIRT, startX, startY, 0, 1, 1, world)
{
}

Dirt::~Dirt()
{
}

void Dirt::doSomething()
{
    return;
}
