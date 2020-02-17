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
    
double Actor::getDistance(double x1, double y1, double x2, double y2) const
{
    double deltaX, deltaY;
    deltaX = pow(x1-x2, 2);
    deltaY = pow(y1-y2, 2);
    return sqrt(deltaX + deltaY);
}

bool Actor::didCollide()
{
    int i = 0;
    vector<Actor*>::iterator it = getWorld()->actorVector().begin();
    if (getActorType() == "Dirt")
    {
        i = getWorld()->numActors("Dirt");
        advance(it, i);
    }
    for (; i < getWorld()->actorVector().size(); i++)
    {
        if (!isDead() && !getWorld()->actorVector()[i]->isDead() && getWorld()->actorVector()[i]->getActorType() != getActorType())
        {
            if (getDistance(getX(), getY(), getWorld()->actorVector()[i]->getX(), getWorld()->actorVector()[i]->getY()) <= 8)
            {
                if (getWorld()->actorVector()[i]->getActorType() == "Dirt" || getWorld()->actorVector()[i]->getActorType() == "Spray")
                    getWorld()->actorVector()[i]->makeDead();
                return true;
            }
        }
     }
    /*int i = 0;
    vector<Actor*>::iterator it = getWorld()->actorVector().begin();
    if (getActorType() == "Dirt")
    {
        i = getWorld()->numActors("Dirt");
        advance(it, i);
    }
    
    for (; it != getWorld()->actorVector().end(); i++)         // for some reason, iterator goes past end
    {
        //cout << i << endl;
        if (i >= getWorld()->actorVector().size())
            break;
         if (!isDead() && !(*it)->isDead() && (*it)->getActorType() != getActorType())
        {
            if (getDistance(getX(), getY(), (*it)->getX(), (*it)->getY()) <= 8)
            {
                if ((*it)->getActorType() == "Dirt" || (*it)->getActorType() == "Spray")
                {
                    cout << (*it)->getActorType() << endl;
                    (*it)->makeDead();
                }
                return true;
            }
        }
        it++;
    }*/
    return false;
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

void Living::makeDead()
{
    m_health = 0;
}

NonLiving::NonLiving(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, size, world)
{
    m_alive = true;
}

NonLiving::~NonLiving()
{
}

void NonLiving::makeDead()
{
    m_alive = false;
}

bool NonLiving::isDead() const
{
    return !m_alive;
}

Socrates::Socrates(StudentWorld* world)
: Living(IID_PLAYER, 0, 128, 0, 0, 1, 100, world)
{
    m_positionalAngle = M_PI;
    m_numSpray = 20;
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
            case KEY_PRESS_SPACE:
                if (m_numSpray > 0)
                {
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    useSpray();
                }
                break;
            case KEY_PRESS_LEFT:
                moveSocrates("left");
                break;
            case KEY_PRESS_RIGHT:
                moveSocrates("right");
                break;
        }
    }
    else                    // no key press
    {
        if (m_numSpray < 20)
        {
            m_numSpray++;
        }
    }
}

string Socrates::getActorType() const
{
    return "Socrates";
}

int Socrates::numSpray() const
{
    return m_numSpray;
}

void Socrates::moveSocrates(string direction)
{
    if (direction == "left")                // pressed left key
    {
        setDirection(getDirection()+5);
        m_positionalAngle += 5 * (M_PI / 180);
        moveTo(VIEW_RADIUS*(cos(m_positionalAngle))+VIEW_RADIUS, VIEW_RADIUS*(sin(m_positionalAngle))+VIEW_RADIUS);
    }
    else                                    // pressed right key
    {
        setDirection(getDirection()-5);
        m_positionalAngle -= 5 * (M_PI / 180);
        moveTo(VIEW_RADIUS*(cos(m_positionalAngle))+VIEW_RADIUS, VIEW_RADIUS*(sin(m_positionalAngle))+VIEW_RADIUS);
    }
}

void Socrates::useSpray()
{
    double dx, dy;
    getPositionInThisDirection(getDirection(),  2*SPRITE_RADIUS, dx, dy);
    getWorld()->addSpray(dx, dy, getDirection());
    m_numSpray--;
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

string Dirt::getActorType() const
{
    return "Dirt";
}

Weapon::Weapon(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* world)
: NonLiving(imageID, startX, startY, dir, depth, size, world)
{
    m_startX = startX;
    m_startY = startY;
}

Weapon::~Weapon()
{
}

double Weapon::distanceTraveled() const
{
    return getDistance(m_startX, m_startY, getX(), getY());
}

bool Weapon::finishedTraveling() const
{
    return maxDistanceAllowed() - distanceTraveled() <= 0;
}

Spray::Spray(double startX, double startY, Direction dir, StudentWorld* world)
: Weapon(IID_SPRAY, startX, startY, dir, 1, 1, world)
{
}

Spray::~Spray()
{
}

int Spray::maxDistanceAllowed() const
{
    return 112;
}

void Spray::doSomething()
{
    if (isDead())
    {
        return;
    }
    moveAngle(getDirection(), SPRITE_RADIUS*2);
    
    if (finishedTraveling())
    {
        makeDead();
    }
}

string Spray::getActorType() const
{
    return "Spray";
}
