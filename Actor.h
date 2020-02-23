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
    virtual std::string getActorType() const = 0;       // for specific Actor Type
    bool isDamageable() const;
    virtual bool isWeapon() const = 0;
    virtual void makeDead() = 0;
    bool didCollide();
    virtual void takeDamage(int damage) = 0;
    
    virtual bool isEnemy() const = 0;
    
    
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
    virtual void makeDead();
    virtual bool isWeapon();
    void restoreHealth();
    virtual void takeDamage(int damage);
    
    
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
    virtual bool isWeapon() const;
    int numSpray() const;
    int numFlame() const;
    void addFiveFlame();
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
    virtual int maxDistanceAllowed() const = 0;
    double distanceTraveled() const;                    // calculate distance traveled from starting point
    bool finishedTraveling() const;                     // returns if weapon has reached max point
    void doSomething();
    virtual bool isWeapon() const;
    
    
private:
    double m_startX;
    double m_startY;
};

class Spray : public Weapon
{
public:
    Spray(double startX, double startY, Direction dir, StudentWorld* world);
    virtual ~Spray();
    virtual int maxDistanceAllowed() const;
    //virtual void doSomething();
    virtual std::string getActorType() const;
};

class Flame : public Weapon
{
public:
    Flame(double startX, double startY, Direction dir, StudentWorld* world);
    virtual ~Flame();
    virtual int maxDistanceAllowed() const;
    //virtual void doSomething();
    virtual std::string getActorType() const;
};

class Goodie : public NonLiving
{
public:
    Goodie(int imageID, StudentWorld* world, int goodieSound, int goodieScore, int angle);
    virtual ~Goodie();
    virtual void doSomething();
    virtual bool isWeapon() const;
    virtual void onContact();
    virtual void goodieAction() = 0;
    
private:
    int m_ticksAlive;
    int m_ticksAllowed; // how many ticks till it despawns
    int m_goodieSound;
    int m_goodieScore;
};

class HealthGoodie : public Goodie
{
public:
    HealthGoodie(StudentWorld* world);
    virtual ~HealthGoodie();
    virtual void goodieAction();
    virtual std::string getActorType() const;
};

class FlameGoodie : public Goodie
{
public:
    FlameGoodie(StudentWorld* world);
    virtual ~FlameGoodie();
    virtual void goodieAction();
    
    virtual std::string getActorType() const;
};

class LifeGoodie : public Goodie
{
public:
    LifeGoodie(StudentWorld* world);
    virtual ~LifeGoodie();
    virtual void goodieAction();
    
    virtual std::string getActorType() const;
};

class Fungus : public Goodie
{
public:
    Fungus(StudentWorld* world);
    virtual ~Fungus();
    virtual void goodieAction();
    
    virtual std::string getActorType() const;
};

class Pit : public NonLiving
{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual ~Pit();
    virtual void doSomething();
    virtual std::string getActorType() const;
    virtual bool isWeapon() const;
    int numBacteriaLeft();

private:
    int m_regSalmonella;
    int m_aggSalmonella;
    int m_ecoli;
    
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
    virtual bool isWeapon() const;
    void setMovePlan(int num);
    virtual bool isEnemy() const;
    
    
protected:
    bool cloneBacteria();
    bool attackSocrates();
    void eatFood();
    int getMovePlan() const;
    void changeMovePlan(int num);
    // movement plan
    // track food
private:
    int m_foodAte;
    int m_movePlan;
};

class Salmonella : public Bacteria
{
public:
    Salmonella(double startX, double startY, StudentWorld* world, int health);
    virtual ~Salmonella();
    virtual bool doesFollowPlayer();
    virtual void doSomething();
    virtual std::string getActorType() const;
    
protected:
    void moveSalmonella();
    void trackFood();
};

class AggSalmonella : public Salmonella
{
public:
    AggSalmonella(double startX, double startY, StudentWorld* world);
    virtual ~AggSalmonella();
    virtual bool doesFollowPlayer();
    virtual std::string getActorType() const;
};

class eColi : public Bacteria
{
public:
    eColi(double startX, double startY, StudentWorld* world);
    virtual ~eColi();
    virtual void doSomething();
    virtual std::string getActorType() const;
    void trackPlayer();

};


#endif // ACTOR_H_
