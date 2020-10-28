#include "StudentWorld.h"
#include "GameConstants.h"

#include "Actor.h"

#include <string>
#include <iterator>
#include <cmath>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    player = nullptr;
}

StudentWorld::~StudentWorld()   // destructor calls cleanup()
{
    cleanUp();
}

int StudentWorld::init()
{
    m_numActors["Pit"] = 0;             // sets tracker of all actors to 0
    m_numActors["Food"] = 0;
    m_numActors["Dirt"] = 0;
    m_numActors["Spray"] = 0;
    m_numActors["Flame"] = 0;
    m_numActors["Health Goodie"] = 0;
    m_numActors["Flame Goodie"] = 0;
    m_numActors["Life Goodie"] = 0;
    m_numActors["Fungus"] = 0;
    m_numActors["Salmonella"] = 0;
    m_numActors["Agg Salmonella"] = 0;
    m_numActors["ecoli"] = 0;
    m_pitsDissolved = 0;
    
    
    player = new Socrates(this);      // place in player, pit, food, and dirt into game
    addPit();
    addFood();
    addDirt();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    addGoodies();
    callDoSomething();              // calls all actors' doSomething functions

    setGameStatText(displayString());
    
    checkDead();
    if (player->isDead())           // check if player is dead
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if (m_pitsDissolved == getLevel() && m_numActors["Salmonella"] + m_numActors["Agg Salmonella"] == 0)    // check if game is over
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = m_actors.begin();     // delete all actors in game
    for (;it != m_actors.end();)
    {
        delete *it;
        it = m_actors.erase(it);
    }

    if (player != nullptr)
    {
        delete player;
        player = nullptr;
    }
}

void StudentWorld::addSpray(double startX, double startY, int dir)  // shoot 1 spray
{
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Dirt"]);
    m_actors.insert(it, new Spray(startX, startY, dir, this));
    m_numActors["Spray"] += 1;
}

void StudentWorld::addFlame(double startX, double startY, int dir)  // shoot 1 flame
{
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Dirt"] + m_numActors["Spray"]);
    m_actors.insert(it, new Flame(startX, startY, dir, this));
    m_numActors["Flame"] += 1;
}

int StudentWorld::numActors(string actorType)
{
    return m_numActors[actorType];
}

Socrates* StudentWorld::getPlayer() const
{
    return player;
}

void StudentWorld::callDoSomething()                // calls doSomething in all actors
{
    checkCollision();
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
        if ((*it)->getActorType() == "Pit" || (*it)->getActorType() == "Food")
            ;
        else
        {
            int i = 0;
            if ((*it)->getActorType() == "Dirt")
            {
                i = numActors("Pit") + numActors("Food") + numActors("Dirt");                                                   // move past things that dirt cannot collide with
            }
            for (; i < m_actors.size(); i++)
            {
                if (((*it)->getActorType() == "Dirt" && m_actors[i]->isEnemy()) ||                                              // dirt and enemies have special collision
                    ((*it)->isEnemy() && m_actors[i]->getActorType() == "Dirt"))
                    ;
                else if (!(*it)->isDead() && !m_actors[i]->isDead() && m_actors[i]->getActorType() != (*it)->getActorType())    // any dead actors or same actor type cannot be compared
                {
                    if (getDistance((*it)->getX(), (*it)->getY(), m_actors[i]->getX(), m_actors[i]->getY()) <= 8)               // check if collides
                    {
                        if (m_actors[i]->getActorType() == "Pit")
                            ;
                        else if (m_actors[i]->getActorType() == "Food")                                                         // pit and food do not collide
                            ;
                        else if ((((*it)->getActorType() == "Health Goodie" ||                                                  // for bacteria and goodies
                                   (*it)->getActorType() == "Flame Goodie"  ||
                                   (*it)->getActorType() == "Life Goodie" ||
                                   (*it)->getActorType() == "Fungus") && m_actors[i]->isEnemy()) ||
                                 ((*it)->isEnemy() && (m_actors[i]->getActorType() == "Health Goodie" ||
                                                        m_actors[i]->getActorType() == "Flame Goodie" ||
                                                        m_actors[i]->getActorType() == "Life Goodie" ||
                                                        m_actors[i]->getActorType() == "Fungus" )))
                            ;
                        else if ((*it)->isWeapon() && m_actors[i]->isEnemy())                                                   // weapons and bacteria collide and deal damage
                        {
                            (*it)->makeDead();
                            if ((*it)->getActorType() == "Spray")
                                m_actors[i]->takeDamage(2);
                            else
                                m_actors[i]->takeDamage(5);
                            if (!m_actors[i]->isDead())
                            {
                                if (m_actors[i]->getActorType() == "eColi")
                                    playSound(SOUND_ECOLI_HURT);
                                else
                                    playSound(SOUND_SALMONELLA_HURT);
                            }
                        }
                        else if (isDamageable(m_actors[i]->getActorType()))                                                     // for weapons and anything it can destroy (dirt, goodies)
                        {
                            m_actors[i]->makeDead();
                            if (isDamageable(m_actors[i]->getActorType()))
                                (*it)->makeDead();
                        }
                    }
                }
            }
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
            if ((*it)->isEnemy())
            {
                increaseScore(100);
                if ((*it)->getActorType() == "ecoli")
                    playSound(SOUND_ECOLI_DIE);
                else
                    playSound(SOUND_SALMONELLA_DIE);
                int n = randInt(1, 2);                 // 50/50 chance of dropping a food when bacteria dies
                if (n == 2)
                {
                    double x, y;
                    x = (*it)->getX();
                    y = (*it)->getY();
                    m_numActors[(*it)->getActorType()] -= 1;
                    delete *it;
                    it = m_actors.erase(it);
                    dropFood(x, y);
                    it = m_actors.begin();          // validates iterator again
                }
                else
                {
                    m_numActors[(*it)->getActorType()] -= 1;    // delete dead actors from game/vector
                    delete *it;
                    it = m_actors.erase(it);
                }
            }
            else
            {
                m_numActors[(*it)->getActorType()] -= 1;
                delete *it;
                it = m_actors.erase(it);
            }
        }
        else
            it++;
    }
}

void StudentWorld::addDirt()
{
    double startX, startY;
    for (int i = 0; i < max(180 - (20*getLevel()), 20); i++)
    {
        getStartPoint(startX, startY);
        
        while (overlapPitFood(startX, startY))              // dirt cannot overlap with pits or food
        {
            getStartPoint(startX, startY);
        }
        
        m_actors.push_back(new Dirt(startX, startY, this));
        m_numActors["Dirt"] += 1;
    }
}

void StudentWorld::addPit()
{
    double startX, startY;
    for (int i = 0; i < getLevel(); i++)
    {
        getStartPoint(startX, startY);
        
        while (overlapPitFood(startX, startY))              // make sure not to overlap with pits or food
        {
            getStartPoint(startX, startY);
        }
        
        m_actors.push_back(new Pit(startX, startY, this));
        m_numActors["Pit"] += 1;
    }
}

void StudentWorld::addFood()
{
    double startX, startY;
    for (int i = 0; i < min(5*getLevel(), 25); i++)
    {
        getStartPoint(startX, startY);
        
        while (overlapPitFood(startX, startY))          // make sure not to overlap with pits or food
        {
            getStartPoint(startX, startY);
        }
        
        m_actors.push_back(new Food(startX, startY, this));
        m_numActors["Food"] += 1;
    }
}

void StudentWorld::dropFood(double startX, double startY)
{
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Pit"]);
    m_actors.insert(it, new Food(startX, startY, this));
    m_numActors["Food"] += 1;
}

bool StudentWorld::findFood(Bacteria* bac, double &foodX, double &foodY)        // check if a food is within a certain distance and gives its position
{
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Pit"]);
    for(; (*it)->getActorType() == "Food"; it++)
    {
        if (getDistance(bac->getX(), bac->getY(), (*it)->getX(), (*it)->getY()) <= 128)
        {
            foodX = (*it)->getX();
            foodY = (*it)->getY();
            return true;
        }
    }
    return false;
}

void StudentWorld::addGoodies()
{
    int chanceFungus = randInt(0, max(510 - getLevel()*10, 200));
    if (chanceFungus == 0)                                           // chance Goodie
    {

        vector<Actor*>::iterator it = m_actors.begin();
        advance(it, m_numActors["Dirt"] + m_numActors["Spray"] + m_numActors["Flame"] + m_numActors["Health Goodie"] + m_numActors["Flame Goodie"] + m_numActors["Life Goodie"]);
        m_actors.insert(it, new Fungus(this));
        m_numActors["Fungus"] += 1;
    }
    
    int chanceGoodie = max(510 - getLevel() * 10, 250);
    if (randInt(0, chanceGoodie - 1) == 0)
    {
        int chooseGoodie = randInt(1, 10);
        if (chooseGoodie <= 6)                      // Health Goodie
        {
            vector<Actor*>::iterator it = m_actors.begin();
            advance(it, m_numActors["Dirt"] + m_numActors["Spray"] + m_numActors["Flame"]);
            m_actors.insert(it, new HealthGoodie(this));
            m_numActors["Health Goodie"] += 1;
        }
        else if (chooseGoodie >= 7 && chooseGoodie < 10)        // Flame Goodie
        {

            vector<Actor*>::iterator it = m_actors.begin();
            advance(it, m_numActors["Dirt"] + m_numActors["Spray"] + m_numActors["Flame"] + m_numActors["Health Goodie"]);
            m_actors.insert(it, new FlameGoodie(this));
            m_numActors["Flame Goodie"] += 1;
        }
        else
        {
            
            vector<Actor*>::iterator it = m_actors.begin();
            advance(it, m_numActors["Dirt"] + m_numActors["Spray"] + m_numActors["Flame"] + m_numActors["Health Goodie"] + m_numActors["Flame Goodie"]);
            m_actors.insert(it, new LifeGoodie(this));
            m_numActors["Life Goodie"] += 1;
        }
    }
}

void StudentWorld::addBacteria(double startX, double startY, string bacType)        // add bacteria into game
{
    if (bacType == "Salmonella")
        m_actors.push_back(new Salmonella(startX, startY, this, 4));
    else if (bacType == "Agg Salmonella")
        m_actors.push_back(new AggSalmonella(startX, startY, this));
    else
        m_actors.push_back(new eColi(startX, startY, this));;
    m_numActors[bacType] += 1;
}



bool StudentWorld::eatFood(Bacteria* bac)                                           // checkis if collides with food and eats any that it does
{
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Pit"]);
    for (; (*it)->getActorType() == "Food"; it++)
    {
        if (getDistance(bac->getX(), bac->getY(), (*it)->getX(), (*it)->getY()) <= SPRITE_WIDTH)
        {
            (*it)->makeDead();
            return true;
        }
    }
    return false;
}

bool StudentWorld::bacBlocking(Bacteria* bac)
{
    double dx, dy;
    bac->getPositionInThisDirection(bac->getDirection(), 3, dx, dy);
    
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Pit"] + m_numActors["Food"]);
    for (; (*it)->getActorType() == "Dirt"; it++)
    {
        if (getDistance(dx, dy, (*it)->getX(), (*it)->getY()) <= SPRITE_WIDTH/2)        // hit dirt
        {
            return true;
        }
    }
    if (getDistance(dx, dy, VIEW_WIDTH/2, VIEW_HEIGHT/2) > VIEW_RADIUS)                 // out of bounds
    {
        return true;
    }
    return false;
}

bool StudentWorld::blockByDirt(Bacteria* bac)
{
    bac->setDirection(atan2(player->getY()-bac->getY(), player->getX()-bac->getX())*180/M_PI);
    
    int n;
    if (bac->getActorType() == "Agg Salmonella")    // agg salmonella
        n = 3;
    else                                            // e coli
        n = 2;
    
    double dx, dy;
    bac->getPositionInThisDirection(bac->getDirection(), n, dx, dy);
    vector<Actor*>::iterator it = m_actors.begin();
    advance(it, m_numActors["Pit"] + m_numActors["Food"]);
    for (; (*it)->getActorType() == "Dirt"; it++)                       // check if it hits dirt
    {
        if (getDistance(dx, dy, (*it)->getX(), (*it)->getY()) <= SPRITE_WIDTH/2)    // hit dirt
            return true;
    }
    return false;
}

string StudentWorld::displayString()    // need to add zeros in front
{
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(0);
    oss << "Score: " << setfill('0') << setw(6) << getScore() << "  ";
    oss << "Level:  " << getLevel() << "  ";
    oss << "Lives: " << getLives() << "  ";
    oss << "Health: " << player->getHealth() << "  ";
    oss << "Sprays: " << player->numSpray() << "  ";
    oss << "Flames:  " << player->numFlame();
    
    string getString = oss.str();
    return getString;
}

bool StudentWorld::overlapPitFood(double startX, double startY)                                 // checks if overlaps with any pits or food
{
    vector<Actor*>::iterator it = m_actors.begin();
    for (int i = 0; i < m_numActors["Pit"] + m_numActors["Food"]; i++, it++)
    {
        if (getDistance((*it)->getX(), (*it)->getY(), startX, startY) <= SPRITE_WIDTH)
            return true;
    }
    return false;
}

void StudentWorld::getStartPoint(double &startX, double &startY)                                // set random spawn point for dirt, pit, food
{
    double startPositionalAngle, startRadius;
    
    double randDec = randInt(0, 100);
    randDec /= 100;
    startRadius = 120 * sqrt(randDec);
    
    randDec = randInt(1, 100);
    randDec /= 100;
    startPositionalAngle = (randDec) * 2 * M_PI;
    
    startX = (startRadius * cos(startPositionalAngle) + VIEW_RADIUS);
    startY = (startRadius * sin(startPositionalAngle) + VIEW_RADIUS);
}

double StudentWorld::getDistance(double x1, double y1, double x2, double y2) const              // gets euclidean distance
{
    double deltaX, deltaY;
    deltaX = pow(x1-x2, 2);
    deltaY = pow(y1-y2, 2);
    return sqrt(deltaX + deltaY);
}

void StudentWorld::dissolvePit()
{
    m_pitsDissolved++;
}

bool StudentWorld::isDamageable(string actorType)
{
    if (actorType == "Dirt" || actorType == "Spray" || actorType == "Flame" || actorType == "Flame Goodie" || actorType == "Health Goodie" || actorType == "Life Goodie" || actorType == "Fungus")
        return true;
    return false;
}
