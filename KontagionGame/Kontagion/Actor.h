#ifndef ACTOR_H_
#define ACTOR_H_

#include <cmath>
#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int MAX_SPRAY = 20;
const int FUNGUS_DAMAGE = 20;
const int ADD_FLAMES = 5;
const int MAX_HEALTH = 100;

using namespace std;

class StudentWorld;


class Actor : public GraphObject
{
public:
    //constructor
    Actor(int imageID, double startX, double startY, Direction dir, int depth, double size)
    : GraphObject(imageID, startX, startY, dir, depth, size)
    {
        m_alive = true;//cout << "Actor!" << endl;
    }
    
    //accessors
    bool isDead() const { return !m_alive; }
    virtual bool isDamageable() const { return true; }
    virtual bool isEdible() const { return false; }
    virtual bool isHarmful() const { return false; }
    virtual bool canBlock() const { return false; }
    
    //mutators
    virtual void takeDamage(int damage) { setDead(); }
    void setDead() { m_alive = false; }
    virtual void doSomething() = 0;
    
    
    
    //destructor
    virtual ~Actor() {}
    
private:
    bool m_alive;
};

class Dirt : public Actor
{
    
public:
    //constructor
    Dirt(double startX, double startY)
    : Actor(IID_DIRT, startX, startY, 0, 1, 1.0) {}
    
    //accessors
    virtual bool canBlock() const { return true; }
    
    //mutators
    virtual void doSomething() {}
    
    
    //destructor
    virtual ~Dirt() {}
private:
    
};

class Socrates : public Actor
{
public:
    //constructor
    Socrates(StudentWorld* world)
    : Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, 1.0)
    {
        m_hit = 100;
        m_spray = 20;
        m_flame = 5;
        m_posAngle = 180;
        m_world = world;
    }
    
    //accessors
    virtual bool isDamageable() const { return false; }
    int getHealth() const { return m_hit; }
    int getSprays() const { return m_spray; }
    int getFlames() const { return m_flame; }
    
    //mutators
    virtual void doSomething();
    virtual void takeDamage(int damage);
    void setHealth(int health) { m_hit = health; }
    void setFlames(int flames) { m_flame = flames; }
    
    
    //destructor
    virtual ~Socrates(){}
    
private:
    int m_hit;
    int m_spray;
    int m_flame;
    int m_posAngle;
    StudentWorld* m_world;
    void updatePos(int key);
};


//UNFINISHED FOOD
class Food : public Actor
{
    
public:
    //constructor
    Food(double startX, double startY)
    : Actor(IID_FOOD, startX, startY, 90, 1, 1.0) {}
    
    //accessors
    virtual bool isDamageable() const { return false; }
    virtual bool isEdible() const { return true; }
    
    //mutators
    virtual void doSomething() {}
    
    
    //destructor
    virtual ~Food() {}
private:
    
};

class Weapon : public Actor
{
public:
    //constructors
    Weapon(int IID, double startX, double startY, Direction dir, int travDis, StudentWorld* world)
    : Actor(IID, startX, startY, dir, 1, 1.0), m_travDis(travDis), m_world(world) {}
    
    //accessors
    virtual bool isDamageable() const { return false; }
    virtual int getDamage() const = 0;
    
    //mutators
    virtual void doSomething();
    
    //destructor
    virtual ~Weapon() {}
private:
    int m_travDis;
    StudentWorld* m_world;
};


class Spray : public Weapon
{
public:
    //constructor
    Spray(double startX, double startY, Direction dir, StudentWorld* world )
    : Weapon(IID_SPRAY, startX, startY, dir, 112, world) {}
    
    //accessors
    virtual int getDamage() const { return 2; }
    
    //destructor
    virtual ~Spray() {}
private:
};

class Flame : public Weapon
{
public:
    //constructor
    Flame(double startX, double startY, Direction dir, StudentWorld* world)
    : Weapon(IID_FLAME, startX, startY, dir, 32, world) {}
    
    //accessors
    virtual int getDamage() const { return 5; }
    
    //destructor
    virtual ~Flame() {}
    
private:
    
};

class Goodie : public Actor
{
public:
    //constructor
    Goodie(int IID, double startX, double startY, int level, StudentWorld* world)
    : Actor(IID, startX, startY, 0, 1, 1.0)
    {
        m_lifetime = max(rand() % (300-10*level), 50);
        m_world = world;
    }
    
    //accessors
    StudentWorld* getWorld() const { return m_world; }
    virtual int getScore() const = 0;
    
    //mutators
    virtual void doSomething();
    virtual void doCustomized() = 0;
    virtual void passOneTick()
    {
        m_lifetime--;
        if (m_lifetime == 0)
            setDead();
    }
    
    //destructor
    virtual ~Goodie() {}
    
private:
    int m_lifetime;
    StudentWorld* m_world;
};

class RestoreHealthGoodie : public Goodie
{
public:
    //constructor
    RestoreHealthGoodie(double startX, double startY, int level, StudentWorld* world)
    : Goodie(IID_RESTORE_HEALTH_GOODIE, startX, startY, level, world) {}
    
    //accessors
    virtual int getScore() const { return 250; }
    
    //mutators
    virtual void doCustomized();
    
    //destructor
    virtual ~RestoreHealthGoodie() {}
};

class FlameThrowerGoodie : public Goodie
{
public:
    //constructor
    FlameThrowerGoodie(double startX, double startY, int level, StudentWorld* world)
    : Goodie(IID_FLAME_THROWER_GOODIE, startX, startY, level, world) {}
    
    //accessors
    virtual int getScore() const { return 300; }
    
    //mutators
    virtual void doCustomized();

    //destructor
    virtual ~FlameThrowerGoodie() {}
};

class ExtraLifeGoodie : public Goodie
{
public:
    //constructor
    ExtraLifeGoodie(double startX, double startY, int level, StudentWorld* world)
    : Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, level, world) {}
    
    //accessors
    virtual int getScore() const { return 500; }
    
    //mutators
    virtual void doCustomized();
    
    //destructor
    virtual ~ExtraLifeGoodie() {}
};

class Fungus : public Goodie
{
public:
    //constructor
    Fungus(double startX, double startY, int level, StudentWorld* world)
    : Goodie(IID_FUNGUS, startX, startY, level, world) {}
    
    //accessors
    virtual int getScore() const { return -50; }
    
    //mutators
    virtual void doCustomized();
    
    //destructor
    virtual ~Fungus() {}
};

class Pit : public Actor
{
public:
    //constructor
    Pit(double startX, double startY, StudentWorld* world)
    : Actor(IID_PIT, startX, startY, 0, 1, 1.0), m_world(world), m_sum(10) {}
    
    //accessors
    virtual bool isDamageable() const { return false; }
    
    //mutators
    virtual void doSomething();
    
    //destructor
    virtual ~Pit() {}
    
private:
    StudentWorld* m_world;
    int m_bacteria[3] = {5, 3, 2};
    int m_sum;
};

class Bacteria : public Actor
{
public:
    //constructor
    Bacteria(int IID, double startX, double startY, int hit, StudentWorld* world)
    : Actor (IID, startX, startY, 90, 0, 1.0), m_hit(hit), m_movePlan(0), m_world(world), m_eaten(0){}
    
    //accessor
    virtual bool isHarmful() const { return true; }
    virtual int getHurtSound() const { return SOUND_SALMONELLA_HURT; }
    virtual int getDieSound() const { return SOUND_SALMONELLA_DIE; }
    virtual StudentWorld* getWorld() const { return m_world; }
    virtual int getDamage() const = 0;
    
    //mutator
    virtual void doSomething() = 0;
    void allBacteriaDo();
    void SalmonellaMove();
    virtual void chasePlayer(bool& skip) {}
    virtual void takeDamage(int damage);
    virtual Bacteria* constructBacteria(double x, double y, StudentWorld* world) = 0;
    
    //destructor
    virtual ~Bacteria() {}
    
private:
    int m_hit;
    int m_movePlan;
    StudentWorld* m_world;
    int m_eaten;
};


class RSalmonella : public Bacteria
{
public:
    //constructor
    RSalmonella(double startX, double startY, StudentWorld* world)
    : Bacteria (IID_SALMONELLA, startX, startY, 4, world){}
    
    //accessors
    virtual int getDamage() const { return 1; }
    
    //mutator
    virtual void doSomething();
    virtual RSalmonella* constructBacteria(double x, double y, StudentWorld* world)
    { return new RSalmonella(x, y, world); }
    
    //destructor
    virtual ~RSalmonella() {}
    
private:
};

class ASalmonella : public Bacteria
{
public:
    //constructor
    ASalmonella(double startX, double startY, StudentWorld* world)
    : Bacteria (IID_SALMONELLA, startX, startY, 10, world){}
    
    //accessors
    virtual int getDamage() const { return 2; }
    
    //mutator
    virtual void doSomething();
    virtual void chasePlayer(bool& skip);
    virtual ASalmonella* constructBacteria(double x, double y, StudentWorld* world)
    { return new ASalmonella(x, y, world); }
    
    //destructor
    virtual ~ASalmonella() {}
private:
};

class Ecoli : public Bacteria
{
public:
    //constructor
    Ecoli(double startX, double startY, StudentWorld* world)
    : Bacteria (IID_ECOLI, startX, startY, 5, world){}
    
    //accessors
    virtual int getHurtSound() const { return SOUND_ECOLI_HURT; }
    virtual int getDieSound() const { return SOUND_ECOLI_DIE; }
    virtual int getDamage() const { return 4; }
    
    //mutators
    virtual void doSomething();
    virtual void chasePlayer(bool& skip);
    virtual Ecoli* constructBacteria(double x, double y, StudentWorld* world)
    { return new Ecoli(x, y, world); }
    
    //destructor
    virtual ~Ecoli() {}
private:
    
};

#endif // ACTOR_H_
