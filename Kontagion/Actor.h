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
    virtual std::string getActorType() const = 0;       // for specific Actor Type
    virtual bool isWeapon() const = 0;
    virtual void makeDead() = 0;
    virtual void takeDamage(int damage) = 0;
    virtual bool isEnemy() const = 0;
    
protected:
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
    virtual void makeDead();
    virtual bool isWeapon() const;
    void restoreHealth();
    virtual void takeDamage(int damage);
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
    virtual void makeDead();          // for collision
    virtual void takeDamage(int damage);
    virtual bool isEnemy() const;
    
private:
    bool m_alive;
};

class Socrates : public Living
{
public:
    Socrates(StudentWorld* world);
    virtual ~Socrates();
    virtual void doSomething();
    virtual std::string getActorType() const;
    int numSpray() const;           // used by set game scoreboard
    int numFlame() const;
    void addFiveFlame();            // used by flame goodie
    virtual bool isEnemy() const;
    
private:
    double m_positionalAngle;
    int m_numSpray;
    int m_numFlame;
    
    void moveSocrates(std::string direction);
    void useSpray();
    void useFlame();

};

class Dirt : public NonLiving
{
public:
    Dirt(double startX, double startY, StudentWorld* world);
    virtual ~Dirt();
    virtual void doSomething();
    virtual std::string getActorType() const;
    virtual bool isWeapon() const;
    
private:
};

class Weapon : public NonLiving
{
public:
    Weapon(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* world);
    virtual ~Weapon();
    void doSomething();
    virtual bool isWeapon() const;
    
protected:
    virtual int maxDistanceAllowed() const = 0;
    double distanceTraveled() const;                    // calculate distance traveled from starting point
    bool finishedTraveling() const;                     // returns if weapon has reached max point
    
private:
    double m_startX;
    double m_startY;
};

class Spray : public Weapon
{
public:
    Spray(double startX, double startY, Direction dir, StudentWorld* world);
    virtual ~Spray();
    virtual std::string getActorType() const;

private:
    virtual int maxDistanceAllowed() const;
};

class Flame : public Weapon
{
public:
    Flame(double startX, double startY, Direction dir, StudentWorld* world);
    virtual ~Flame();
    virtual std::string getActorType() const;
    
private:
    virtual int maxDistanceAllowed() const;
};

class Goodie : public NonLiving
{
public:
    Goodie(int imageID, StudentWorld* world, int goodieSound, int goodieScore, int angle);
    virtual ~Goodie();
    virtual void doSomething();
    virtual bool isWeapon() const;
    virtual void goodieAction() = 0;
    
private:
    int m_ticksAlive;
    int m_ticksAllowed; // how many ticks till it despawns
    int m_goodieSound;
    int m_goodieScore;
    void onContact();
};

class HealthGoodie : public Goodie
{
public:
    HealthGoodie(StudentWorld* world);
    virtual ~HealthGoodie();
    virtual std::string getActorType() const;
    
private:
    virtual void goodieAction();
};

class FlameGoodie : public Goodie
{
public:
    FlameGoodie(StudentWorld* world);
    virtual ~FlameGoodie();
    virtual std::string getActorType() const;
    
private:
    virtual void goodieAction();
};

class LifeGoodie : public Goodie
{
public:
    LifeGoodie(StudentWorld* world);
    virtual ~LifeGoodie();
    virtual std::string getActorType() const;
    
private:
    virtual void goodieAction();
};

class Fungus : public Goodie
{
public:
    Fungus(StudentWorld* world);
    virtual ~Fungus();
    virtual std::string getActorType() const;
    
private:
    virtual void goodieAction();
};

class Pit : public NonLiving
{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual ~Pit();
    virtual void doSomething();
    virtual std::string getActorType() const;
    virtual bool isWeapon() const;

private:
    int m_regSalmonella;
    int m_aggSalmonella;
    int m_ecoli;
    int numBacteriaLeft();
};

class Food : public NonLiving
{
public:
    Food(double startX, double startY, StudentWorld* world);
    virtual ~Food();
    virtual void doSomething();
    virtual std::string getActorType() const;
    virtual bool isWeapon() const;
};

class Bacteria : public Living
{
public:
    Bacteria(int imageID, double startX, double startY, int health, StudentWorld* world);
    virtual ~Bacteria();
    virtual bool isEnemy() const;
    
protected:
    void setMovePlan(int num);
    bool cloneBacteria();
    bool attackSocrates();
    void eatFood();
    int getMovePlan() const;
    void changeMovePlan(int num);
    bool findPlayer();
    
private:
    int m_foodAte;
    int m_movePlan;
};

class Salmonella : public Bacteria
{
public:
    Salmonella(double startX, double startY, StudentWorld* world, int health);
    virtual ~Salmonella();
    virtual void doSomething();
    virtual std::string getActorType() const;
    
protected:
    virtual bool doesFollowPlayer();
    void moveSalmonella();
    void trackFood();
};

class AggSalmonella : public Salmonella
{
public:
    AggSalmonella(double startX, double startY, StudentWorld* world);
    virtual ~AggSalmonella();
    virtual std::string getActorType() const;
private:
    virtual bool doesFollowPlayer();
};

class eColi : public Bacteria
{
public:
    eColi(double startX, double startY, StudentWorld* world);
    virtual ~eColi();
    virtual void doSomething();
    virtual std::string getActorType() const;
private:
    void trackPlayer();
};


#endif // ACTOR_H_
