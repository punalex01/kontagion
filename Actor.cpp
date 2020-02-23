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

bool Actor::didCollide() // move to studentWorld 
{
    /*int i = 0;
    vector<Actor*>::iterator it = getWorld()->actorVector().begin();
    if (getActorType() == "Dirt")
    {
        i = getWorld()->numActors("Pit") + getWorld()->numActors("Food") + getWorld()->numActors("Dirt");
        advance(it, i);
    }
    for (; i < getWorld()->actorVector().size(); i++)
    {
        if (!isDead() && !getWorld()->actorVector()[i]->isDead() && getWorld()->actorVector()[i]->getActorType() != getActorType())
        {
            if (getDistance(getX(), getY(), getWorld()->actorVector()[i]->getX(), getWorld()->actorVector()[i]->getY()) <= 8)
            {
                if (getWorld()->actorVector()[i]->getActorType() == "Pit")
                    return false;
                else if (getWorld()->actorVector()[i]->getActorType() == "Food")    // need to change later for bac
                    return false;
                else if (getWorld()->actorVector()[i]->isDamageable())
                    getWorld()->actorVector()[i]->makeDead();
                return true;
            }
        }
     }
    return false;*/
    return true;
}

bool Actor::isDamageable() const
{
    if (getActorType() == "Dirt" || getActorType() == "Spray" || getActorType() == "Flame" || getActorType() == "Flame Goodie" || getActorType() == "Health Goodie" || getActorType() == "Life Goodie" || getActorType() == "Fungus")
        return true;
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
    return m_health <= 0;
}

int Living::getHealth() const
{
    return m_health;
}

void Living::addHealth(int health)
{
    m_health += health;
}

void Living::restoreHealth()
{
    m_health = 100;
}

void Living::makeDead()
{
    m_health = 0;
}

bool Living::isWeapon()
{
    return false;
}

void Living::takeDamage(int damage)
{
    m_health -= damage;
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

void NonLiving::takeDamage(int damage)
{
    return;
}

bool NonLiving::isEnemy() const
{
    return false;
}

Socrates::Socrates(StudentWorld* world)
: Living(IID_PLAYER, 0, 128, 0, 0, 1, 100, world)
{
    m_positionalAngle = M_PI;
    m_numSpray = 20;
    m_numFlame = 5;
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
                    useSpray();
                break;
            case KEY_PRESS_ENTER:
                if (m_numFlame > 0)
                    useFlame();
                break;
            case KEY_PRESS_LEFT:
                moveSocrates("left");
                break;
            case KEY_PRESS_RIGHT:
                moveSocrates("right");
                break;
            case KEY_PRESS_UP:
                getWorld()->addBacteria(128, 0, "eColi");
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

int Socrates::numFlame() const
{
    return m_numFlame;
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
    getWorld()->playSound(SOUND_PLAYER_SPRAY);
    double dx, dy;
    getPositionInThisDirection(getDirection(),  2*SPRITE_RADIUS, dx, dy);
    getWorld()->addSpray(dx, dy, getDirection());
    m_numSpray--;
}

void Socrates::useFlame()
{
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    double dx, dy;
    for (int i = 0; i <= 15; i++)
    {
        getPositionInThisDirection(getDirection()+(22*i), 2*SPRITE_RADIUS, dx, dy);
        getWorld()->addFlame(dx, dy, getDirection() + (22*i));
    }
    m_numFlame--;
}

bool Socrates::isWeapon() const
{
    return false;
}

void Socrates::addFiveFlame()
{
    m_numFlame += 5;
}

bool Socrates::isEnemy() const
{
    return false;
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

bool Dirt::isWeapon() const
{
    return false;
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
    return getWorld()->getDistance(m_startX, m_startY, getX(), getY());
}

bool Weapon::finishedTraveling() const
{
    return maxDistanceAllowed() - distanceTraveled() <= 0;
}

void Weapon::doSomething()
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

bool Weapon::isWeapon() const
{
    return true;
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

//void Spray::doSomething()
//{
//}

string Spray::getActorType() const
{
    return "Spray";
}

Flame::Flame(double startX, double startY, Direction dir, StudentWorld* world)
: Weapon(IID_FLAME, startX, startY, dir, 1, 1, world)
{
}

Flame::~Flame()
{
}

int Flame::maxDistanceAllowed() const
{
    return 32;
}

//void Flame::doSomething()

string Flame::getActorType() const
{
    return "Flame";
}

Goodie::Goodie(int imageID, StudentWorld* world, int goodieSound, int goodieScore, int angle = (128 * cos(randInt(1, 360) * (M_PI/180)) + VIEW_RADIUS))
: NonLiving(imageID, (128 * cos(angle * (M_PI/180)) + VIEW_RADIUS), (128 * sin(angle * (M_PI/180)) + VIEW_RADIUS), 0, 1, 1, world)
{
    m_ticksAlive = 0;
    m_ticksAllowed = max(rand() % (300 - 10 * getWorld()->getLevel()), 50);
    m_goodieSound = goodieSound;
    m_goodieScore = goodieScore;
}

Goodie::~Goodie()
{
}

void Goodie::doSomething()
{
    if (m_ticksAlive >= m_ticksAllowed)
        makeDead();
    if (isDead())
        return;
    if (getWorld()->getDistance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) <= 8)
    {
        onContact();
        return;
    }
    m_ticksAlive++;
}

void Goodie::onContact()
{
    getWorld()->increaseScore(m_goodieScore);
    makeDead();
    getWorld()->playSound(m_goodieSound);
    goodieAction();
}

bool Goodie::isWeapon() const
{
    return false;
}

HealthGoodie::HealthGoodie(StudentWorld* world)
: Goodie(IID_RESTORE_HEALTH_GOODIE, world, SOUND_GOT_GOODIE, 250)
{
}

HealthGoodie::~HealthGoodie()
{
}

void HealthGoodie::goodieAction()
{
    getWorld()->getPlayer()->restoreHealth();
}

string HealthGoodie::getActorType() const
{
    return "Health Goodie";
}

FlameGoodie::FlameGoodie(StudentWorld* world)
: Goodie(IID_FLAME_THROWER_GOODIE, world, SOUND_GOT_GOODIE, 300)
{
}

FlameGoodie::~FlameGoodie()
{
}

void FlameGoodie::goodieAction()
{
    getWorld()->getPlayer()->addFiveFlame();
}

string FlameGoodie::getActorType() const
{
    return "Flame Goodie";
}

LifeGoodie::LifeGoodie(StudentWorld* world)
: Goodie(IID_EXTRA_LIFE_GOODIE, world, SOUND_GOT_GOODIE, 500)
{
}

LifeGoodie::~LifeGoodie()
{
}
void LifeGoodie::goodieAction()
{
    getWorld()->incLives();
}

std::string LifeGoodie::getActorType() const
{
    return "Life Goodie";
}

Fungus::Fungus(StudentWorld* world)
: Goodie(IID_FUNGUS, world, SOUND_NONE, -50)
{
}

Fungus::~Fungus()
{
}

string Fungus::getActorType() const
{
    return "Fungus";
}

void Fungus::goodieAction()
{
    getWorld()->getPlayer()->addHealth(-20);
}

double randRadius()
{
    return randInt(1, 360);
}

Pit::Pit(double startX, double startY, StudentWorld* world)
: NonLiving(IID_PIT, startX, startY, 0, 1, 1, world)
{
    m_regSalmonella = 5;
    m_aggSalmonella = 3;
    m_ecoli = 2;
}

Pit::~Pit()
{
}

void Pit::doSomething()
{
    if (numBacteriaLeft() == 0)
    {
        makeDead();
        getWorld()->dissolvePit();
        return;
    }
    if (randInt(1, 50) == 1)
    {
        int randNum = randInt(1, numBacteriaLeft());
        if (randNum <= m_regSalmonella)
        {
            getWorld()->addBacteria(getX(), getY(), "Salmonella");
            m_regSalmonella--;
        }
        else if (randNum > m_regSalmonella && randNum <= m_regSalmonella + m_aggSalmonella)
        {
            getWorld()->addBacteria(getX(), getY(), "Agg Salmonella");
            m_aggSalmonella--;
        }
        else
        {
            getWorld()->addBacteria(getX(), getY(), "eColi");
            m_ecoli--;
        }
        getWorld()->playSound(SOUND_BACTERIUM_BORN);
    }
}

string Pit::getActorType() const
{
    return "Pit";
}

bool Pit::isWeapon() const
{
    return false;
}

int Pit::numBacteriaLeft()
{
    return m_regSalmonella + m_aggSalmonella + m_ecoli;
}

Food::Food(double startX, double startY, StudentWorld* world)
: NonLiving(IID_FOOD, startX, startY, 90, 1, 1, world)
{
}

Food::~Food()
{
}

void Food::doSomething()
{
}

string Food::getActorType() const
{
    return "Food";
}

bool Food::isWeapon() const
{
    return false;
}

Bacteria::Bacteria(int imageID, double startX, double startY, int health, StudentWorld* world)
: Living(imageID, startX, startY, 90, 0, 1, health, world)
{
    m_foodAte = 0;
    m_movePlan = 0;
}

Bacteria::~Bacteria()
{
}

bool Bacteria::isWeapon() const
{
    return false;
}

bool Bacteria::cloneBacteria()
{
    if (m_foodAte == 3)
    {
        double newX, newY;
        if (getX() < VIEW_HEIGHT/2)
            newX = getX() + SPRITE_WIDTH/2;
        else if (getX() > VIEW_HEIGHT/2)
            newX = getX() - SPRITE_WIDTH/2;
        else
            newX = getX();
        
        if (getY() < VIEW_HEIGHT/2)
            newY = getY() + SPRITE_WIDTH/2;
        else if (getY() > VIEW_HEIGHT/2)
            newY = getY() - SPRITE_WIDTH/2;
        else
            newY = getY();
        
        getWorld()->addBacteria(newX, newY, getActorType());
        m_foodAte = 0;
        return true;
    }
    return false;
}

bool Bacteria::attackSocrates()
{
    if (getWorld()->getDistance(getX(), getY(),
        getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) <= SPRITE_WIDTH)
    {
        if (getActorType() == "Salmonella")
            getWorld()->getPlayer()->addHealth(-1);
        else if (getActorType() == "Aggressive Salmonella")
            getWorld()->getPlayer()->addHealth(-2);
        else
            getWorld()->getPlayer()->addHealth(-4);
        return true;
    }
    return false;
}

void Bacteria::eatFood()
{
    if (getWorld()->eatFood(this))
    {
        m_foodAte++;
    }
}

void Bacteria::changeMovePlan(int num)
{
    m_movePlan += num;
}

int Bacteria::getMovePlan() const
{
    return m_movePlan;
}

void Bacteria::setMovePlan(int num)
{
    m_movePlan = num;
}

bool Bacteria::isEnemy() const
{
    return true;
}

Salmonella::Salmonella(double startX, double startY, StudentWorld* world, int health = 4)
: Bacteria(IID_SALMONELLA, startX, startY, health, world)
{
}

Salmonella::~Salmonella()
{
    
}

bool Salmonella::doesFollowPlayer()
{
    return false;
}

void Salmonella::doSomething()
{
    if (isDead())       // step 1
        return;
    if (doesFollowPlayer()) // for aggressive salmonella
    {
        if (attackSocrates())
            return;
        cloneBacteria();
        eatFood();
        return;
    }
        
    if (attackSocrates())   // step 2
    {
        moveSalmonella();
        return;
    }
    if (cloneBacteria())    // step 3
    {
        moveSalmonella();
        return;
    }
    eatFood();          // step 4
    moveSalmonella();   // step 5/6
    return;
}

void Salmonella::moveSalmonella()
{
    if (getMovePlan() > 0)
    {
        changeMovePlan(-1);
        if (getWorld()->bacBlocking(this))
        {
            int randDir = randInt(0, 359);
            setDirection(randDir);
            setMovePlan(10);
        }
        else
        {
            moveAngle(getDirection(), 3);
        }
        return;
    }
    else
    {
        trackFood();
    }
}

string Salmonella::getActorType() const
{
    return "Salmonella";
}

void Salmonella::trackFood()
{
    double foodX = 0;
    double foodY = 0;
    if (getWorld()->findFood(this, foodX, foodY))
    {
        setDirection(atan2(foodY-getY(), foodX-getX())*180/M_PI);
        if (getWorld()->bacBlocking(this))
        {
            int randDir = randInt(0, 359);
            setDirection(randDir);
            setMovePlan(10);
        }
        else
        {
            moveAngle(getDirection(), 3);
        }
    }
    else
    {
        int randDir = randInt(0, 359);
        setDirection(randDir);
        setMovePlan(10);
    }
}

AggSalmonella::AggSalmonella(double startX, double startY, StudentWorld* world)
: Salmonella(startX, startY, world, 10)
{
}

AggSalmonella::~AggSalmonella()
{
}

string AggSalmonella::getActorType() const
{
    return "Agg Salmonella";
}

bool AggSalmonella::doesFollowPlayer()
{
    if (getWorld()->trackPlayer(this, "Agg Salmonella"))
    {
        if (getWorld()->blockByDirt(this))
            return true;
        moveAngle(getDirection(), 3);
        return true;
    }
    return false;
}

eColi::eColi(double startX, double startY, StudentWorld* world)
: Bacteria(IID_ECOLI, startX, startY, 5, world)
{
}

eColi::~eColi()
{
}

string eColi::getActorType() const
{
    return "eColi";
}

void eColi::doSomething()
{
    if (isDead())
        return;
    
    if (attackSocrates())   // step 2
    {
        trackPlayer();
        return;
    }
    if (cloneBacteria())    // step 3
    {
        trackPlayer();
        return;
    }
    
    eatFood();
    trackPlayer();
    return;
}

void eColi::trackPlayer()
{
    if (getWorld()->trackPlayer(this, "eColi"))     // step 5
    {
        for (int i = 0; i < 10; i++)
        {
            if (getWorld()->blockByDirt(this))
            {
                setDirection(getDirection()+10);
            }
            else
            {
                moveForward(2);
                return;
            }
        }
    }
}


