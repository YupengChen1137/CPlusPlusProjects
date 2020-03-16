#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

///////////////////////////////////////////////////////////////////////////
//  Key functions implementation
///////////////////////////////////////////////////////////////////////////

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_player = nullptr;
    m_clearPit = 1;
}

int StudentWorld::init()
{
    setClear(getLevel());
    
    //Initialize Socrates
    Socrates* player = new Socrates(this);
    m_player = player;
    
    addDirt();
    addPit();
    addFood();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //player will do something
    m_player->doSomething();
    
    //each actor will do something
    for(list<Actor*>::iterator p = m_AList.begin(); p != m_AList.end(); p++)
    {
        (*p)->doSomething();
        
        //if player is dead
        //return GWSTATUS_PLAYER_DIED
        if (m_player->isDead())
        {
            //NEED TO CONFIRM
            decLives();
            playSound(SOUND_PLAYER_DIE);
            delete m_player;
            return GWSTATUS_PLAYER_DIED;
        }
        
        //else if (number of bacteria + number of pits ==0)
        //return GWSTATUS_FINISHED_LEVEL;
        else if (getNumBacteria() == 0 && isClear())
        {
            delete m_player;
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    //clear died actors
    list<Actor*>::iterator p = m_AList.begin();
    while(p != m_AList.end())
    {
        if((*p)->isDead())
        {
            delete *p;
            p = m_AList.erase(p);
        }
        p++;
    }
    
    //It must then add any new objects to the game (e.g., a new goodie or fungus).
    addFungus();
    addGoodies();
    
    //It must update the status text on the top of the screen with the latest information
    //      (e.g., the user’s current score, the number of foods Socrates has, the current level, etc.).
    string stat = getStatText(getScore(), getLevel(), getLives(), m_player->getHealth(), m_player->getSprays(), m_player->getFlames());
    setGameStatText(stat);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator p = m_AList.begin();
    while(p!=m_AList.end())
    {
        //free the memory and then erase the actor pointer from the list
        delete *p;
        p = m_AList.erase(p);
    }
}


StudentWorld::~StudentWorld()
{
    cleanUp();
}


///////////////////////////////////////////////////////////////////////////
//  Public implementation
///////////////////////////////////////////////////////////////////////////

//add the Actor pointer to the list
void StudentWorld::addToList(Actor* added)
{
    m_AList.push_back(added);
}

//set player's hitpoints to be health
void StudentWorld::setPlayerHealth(int health)
{
    m_player->setHealth(health);
}

//add flames number of flame to the player's arsenal
void StudentWorld::addPlayerFlames(int flames)
{
    m_player->setFlames(m_player->getFlames() + flames);
}

//damage the player
void StudentWorld::damagePlayer(int damage)
{
    m_player->takeDamage(damage);
}

//check to see if Socrates is within certain range
bool StudentWorld::isPlayerNear(double x, double y, int dis)
{
    double distance = getDistance(x, y, m_player->getX(), m_player->getY());
    return (distance <= dis*dis);
}

//check to see if a weapon overlaps with damageable object
//if so, damage the object and return true
//if not, do nothing and return false
bool StudentWorld::weaponDamage(double xw, double yw, int damageAmount)
{
    list<Actor*>::iterator p = m_AList.begin();
    while(p != m_AList.end())
    {
        if (!(*p)->isDead())
        {
            double dis = getDistance(xw, yw, (*p)->getX(), (*p)->getY());
            if (dis <= 64 && (*p)->isDamageable())
            {
                (*p)->takeDamage(damageAmount);
                return true;
            }
        }
        p++;
    }
    return false;
}

//check to see if a bacteria overlaps with any food
//if so, eat the food and return true
//if not, do nothing and return false
bool StudentWorld::bacteriaEat(double xb, double yb, int& eaten)
{
    list<Actor*>::iterator p = m_AList.begin();
    while(p != m_AList.end())
    {
        if (!(*p)->isDead())
        {
            double dis = getDistance(xb, yb, (*p)->getX(), (*p)->getY());
            if ((*p)->isEdible() && dis <= 64 )
            {
                (*p)->takeDamage(1);
                eaten++;
                return true;
            }
        }
        p++;
    }
    return false;
}

//check to see if the movement of a bacteria is blocked
//input: starting position and ending position of the movement
bool StudentWorld::bacteriaBlocked(double xs, double ys, double xe, double ye)
{
    list<Actor*>::iterator p = m_AList.begin();
    while(p != m_AList.end())
    {
        double disStart = getDistance(xs, ys, (*p)->getX(), (*p)->getY());;
        double disEnd = getDistance(xe, ye, (*p)->getX(), (*p)->getY());;
        double disLegit = SPRITE_WIDTH*SPRITE_WIDTH/4;
        if ((*p)->canBlock() && (disStart <= disLegit || disEnd <= disLegit))
            return true;
        p++;
    }
    return false;
}

//Set dir to be the directional angle to the angle to the player.
void StudentWorld::findPlayer(double x, double y, int& dir)
{
    if (x == m_player->getX() && y == m_player->getY())
        return;
    dir = getDirectionalAngle(x, y, m_player->getX(), m_player->getY());
}

//Find if there's a food object within the distance of VIEW_RADIUS
//Set dir to be the directional angle to the angle to the food.
bool StudentWorld::findPizza(double x, double y, int& dir)
{
    list<Actor*>::iterator p = m_AList.begin();
    while(p != m_AList.end())
    {
        if ((*p)->isEdible() && getDistance((*p)->getX(), (*p)->getY(), x, y) < VIEW_RADIUS*VIEW_RADIUS)
        {
            dir = getDirectionalAngle(x, y, (*p)->getX(), (*p)->getY());
            return true;
        }
        p++;
    }
    return false;
}

//Count the number of alive bacteria in the petri dish
//Return 0 if there's no alive bacteria
int StudentWorld::getNumBacteria()
{
    int count = 0;
    list<Actor*>::iterator p = m_AList.begin();
    while(p != m_AList.end())
    {
        if((*p)->isHarmful() && !(*p)->isDead())
            count++;
        p++;
    }
    return count;
}

///////////////////////////////////////////////////////////////////////////
//  Private helper implementation
///////////////////////////////////////////////////////////////////////////

//return a valid string display
string StudentWorld::getStatText(int Score, int Level, int Lives, int health, int Sprays, int Flames)
{
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(0);
    
    //handle negative scores
    if (Score < 0)
    {
        oss.fill('0');
        oss << "Score: -" << setw(5) << abs(Score) << "  ";
    }
    
    //handle positive scores
    else
    {
        oss.fill('0');
        oss << "Score: " << setw(6) << Score << "  ";
    }
    
    //others
    oss << "Level: " << Level << "  ";
    oss << "Lives: " << Lives << "  ";
    oss << "health: " << health << "  ";
    oss << "Sprays: " << Sprays << "  ";
    oss << "Flames: " << Flames;
    
    string output = oss.str();
    return output;
}

//find a random position on the border of the petri dish
void StudentWorld::randPositionOnRadius(double& x, double& y)
{
    Direction generateAngle = randInt(0,359);
    const double PI = 4 * atan(1);
    x = (VIEW_WIDTH/2 + VIEW_RADIUS * cos(generateAngle*1.0 / 360 * 2 * PI));
    y = (VIEW_HEIGHT/2 + VIEW_RADIUS * sin(generateAngle*1.0 / 360 * 2 * PI));
}

//determine if an actor is overlapped with some other actors
bool StudentWorld::isOverlap(double x, double y)
{
    list<Actor*>::iterator p = m_AList.begin();
    while(p != m_AList.end())
    {
        double dis = getDistance(x, y, (*p)->getX(), (*p)->getY());
        if (dis <= 4*SPRITE_RADIUS*SPRITE_RADIUS)
        {
            return true;
        }
        p++;
    }
    
    return false;
}

//return the squared distance between two pairs of coordinates
double StudentWorld::getDistance(double X, double Y, double x, double y)
{
    return (X-x)*(X-x)+(Y-y)*(Y-y);
}

//get the directional angle from (X,Y) to (x,y)
int StudentWorld::getDirectionalAngle(double X, double Y, double xp, double yp)
{
    
    const double PI = 4 * atan(1);
//    if (X < xp)
//        return (360 + int(atan((Y-yp)/(X-xp))*180/PI)) % 360;
//    else if (X > xp)
//        return int(atan((Y-yp)/(X-xp))*180/PI) + 180;
//    else if (Y > yp)
//    {
//        return 270;
//    }
//    else
//    {
//        return 90;
//    }
    
//    int angle;
//    if (yp > Y)
//    {
//        angle = 90 - atan((xp-X)/(yp-Y))*180/PI;
//        return angle;
//    }
//    else if (yp < Y)
//    {
//        angle = 270 + atan((xp-X)/(yp-Y))*180/PI;
//        return angle;
//    }
//    else if (xp > X)
//        return 0;
//    else
//        return 180;
    
    int angle;
    if (yp > Y)
    {
        angle = 90 - atan((X-xp)/(Y-yp))*180/PI;
        return angle;
    }
    else if (yp < Y)
    {
        angle = 270 - atan((X-xp)/(Y-yp))*180/PI;
        return angle;
    }
    else if (xp > X)
        return 0;
    else
        return 180;
}

//return a random position inside the petri dish
void StudentWorld::randPositionInside(double &x, double &y)
{
    x = randInt(0, 255);
    y = randInt(0, 255);
    while (getDistance(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2) >= 14400)
    {
        x = randInt(0, 255);
        y = randInt(0, 255);
    }
}



///////////////////////////////////////////////////////////////////////////
//  More Private implementation
///////////////////////////////////////////////////////////////////////////

//Add L pits to the Petri dish at random locations, in a manner such that no two pits overlap with each other (their centers are at least 2*SPRITE_RADIUS pixels apart from each other). Each pit must be no more 120 pixels from the center of the Petri dish which is at (VIEW_WIDTH/2, VIEW_HEIGHT/2)
void StudentWorld::addPit()
{
    for (int i = 0; i < getLevel(); i++)
    {
        double pitX, pitY;
        randPositionInside(pitX, pitY);
        if(!isOverlap(pitX, pitY))
            m_AList.push_back(new Pit(pitX, pitY, this));
        else
            i--;
    }
}

//Add max(180 – 20 * L, 20) dirt objects to the Petri dish at random locations, in a manner such that no dirt objects overlap with previously-placed food objects or pits (their centers are at least 2*SPRITE_RADIUS pixels apart from each other). It is OK for dirt objects to overlap with each other, however. Each dirt object must be no more 120 pixels from the center of the Petri dish which is at (VIEW_WIDTH/2, VIEW_HEIGHT/2).
void StudentWorld::addDirt()
{
    for (int i = 0; i < max(180 - 20*getLevel(), 20); i++)
    {
        double dirtX, dirtY;
        randPositionInside(dirtX, dirtY);
        m_AList.push_back(new Dirt(dirtX, dirtY));
    }
}

//add fungus to the game.
void StudentWorld::addFungus()
{
    int ChanceFungus = min(510 - getLevel() * 10, 200);
    if (randInt(0, ChanceFungus-1) == 0)
    {
        double newX, newY;
        randPositionOnRadius(newX, newY);
        m_AList.push_back(new Fungus(newX, newY, getLevel(), this));
    }
}

//add goodies to the game.
void StudentWorld::addGoodies()
{
    int ChanceGoodie = min(510 - getLevel() * 10, 250);
    if (randInt(0, ChanceGoodie-1) == 0)
    {
        double newX, newY;
        randPositionOnRadius(newX, newY);
        int goodieType = randInt(0, 9);
        Actor* addedGoodie;
        if (goodieType < 6)
            addedGoodie = new RestoreHealthGoodie(newX, newY, getLevel(), this);
        else if (goodieType < 9)
            addedGoodie = new FlameThrowerGoodie(newX, newY, getLevel(), this);
        else
            addedGoodie = new ExtraLifeGoodie(newX, newY, getLevel(), this);
        m_AList.push_back(addedGoodie);
    }
}

//Add min(5 * L, 25) food objects to the Petri dish at random locations, in a manner such that no two food objects overlap with each other or previously-placed pits (their centers must be at least 2*SPRITE_RADIUS pixels apart from each other). Each food object must be no more 120 pixels from the center of the Petri dish which is at (VIEW_WIDTH/2, VIEW_HEIGHT/2).
void StudentWorld::addFood()
{
    for (int i = 0; i < min(5*getLevel(), 25); i++)
    {
        double foodX, foodY;
        randPositionInside(foodX, foodY);
        if (!isOverlap(foodX, foodY))
            m_AList.push_back(new Food(foodX, foodY));
        else
            i--;
    }
}
