#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Socrates;
class Actor;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    
    //accessors
    bool isClear() { return m_clearPit == 0; }
    int getNumBacteria();
    
    //locators
    bool bacteriaBlocked(double xs, double ys, double xe, double ye);
    bool isOverlap(double x, double y);
    bool isPlayerNear(double x, double y, int dis);
    bool findPizza(double x, double y, int& dir);
    void findPlayer(double x, double y, int& dir);
    
    //mutators
    void addToList(Actor* added);
    void setClear(int clear) { m_clearPit = clear; }
    void decClear() { m_clearPit--; }
    void setPlayerHealth(int health);
    void addPlayerFlames(int flames);
    void damagePlayer(int damage);
    bool weaponDamage(double xw, double yw, int damageAmount);
    bool bacteriaEat(double xb, double yb, int& eaten);
    

private:
    Socrates* m_player;
    list<Actor*> m_AList;
    int m_clearPit;
    
    //Helpers
    string getStatText(int Score, int Level, int Lives, int health, int Sprays, int Flames);
    void randPositionOnRadius(double& x, double& y);
    void randPositionInside(double &x, double &y);
    double getDistance(double X, double Y, double x, double y);
    int getDirectionalAngle(double X, double Y, double x, double y);
    
    //Mutators
    void addPit();
    void addDirt();
    void addFood();
    void addFungus();
    void addGoodies();
};
#endif // STUDENTWORLD_H_
