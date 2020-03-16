#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


///////////////////////////////////////////////////////////////////////////
//  Socrates implementation
///////////////////////////////////////////////////////////////////////////
void Socrates::doSomething()
{
    if(this->isDead())
    {
        return;
    }
    
    int key = 0;
    if (m_world->getKey(key))
    {
        
        switch(key)
        {
            case KEY_PRESS_LEFT:
                updatePos(key);
                break;
            case KEY_PRESS_RIGHT:
                updatePos(key);
                break;
            case KEY_PRESS_SPACE:
                if(m_spray > 0)
                {
                    double x,y;
                    getPositionInThisDirection(getDirection(), SPRITE_RADIUS*2, x, y);
                    Spray* firedSpray = new Spray(x, y, getDirection(), m_world);
                    m_world->addToList(firedSpray);
                    m_spray--;
                    //play the SOUND_PLAYER_SPRAY sound effect
                    m_world->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            case KEY_PRESS_ENTER:
                if(m_flame > 0)
                {
                    //Add 16 flame objects exactly 2*SPRITE_RADIUS pixels away from himself, starting directly in front of Socrates and in 22 degree increments around Socrates (making a complete circle of flames around Socrates). Each flame object must face the same direction/angle that it was created around Socrates. e.g., a flame object created at 44 degrees from the horizontal should have a direction it faces of 44 degrees.
                    for (int i = 0; i < 16; i++)
                    {
                        Direction degree = i*22;
                        double u,v;
                        getPositionInThisDirection(degree, SPRITE_RADIUS/2, u, v);
                        Flame* firedFlame = new Flame(u, v, degree, m_world);
                        m_world->addToList(firedFlame);
                    }
                    m_flame--;
                    //play the SOUND_PLAYER_FLAME sound effect
                    m_world->playSound(SOUND_PLAYER_FIRE);
                    
                }
                break;
            default:
                break;
        }
    }
    else if (m_spray < MAX_SPRAY)
        m_spray++;
}

//called when Socrates is damaged, plays the sound and decrease hit points
void Socrates::takeDamage(int damage)
{
    m_hit -= damage;
    if (m_hit > 0)
    {
        m_world->playSound(SOUND_PLAYER_HURT);
    }
    else
    {
        setDead();
        m_world->playSound(SOUND_PLAYER_DIE);
    }
}

//called when Socrates trying to move, update it's position based on the direction key
void Socrates::updatePos(int key)
{
    int newPosAngle;
    if (key == KEY_PRESS_RIGHT)
        newPosAngle = m_posAngle - 5;
    else if (key == KEY_PRESS_LEFT)
        newPosAngle = m_posAngle + 5;
    else
        return;
    m_posAngle = (newPosAngle+360) % 360;
    
    const double PI = 4 * atan(1);
    double newX;
    newX = (VIEW_WIDTH/2 + VIEW_RADIUS * cos(m_posAngle*1.0 / 360 * 2 * PI));
    double newY;
    newY = (VIEW_HEIGHT/2 + VIEW_RADIUS * sin(m_posAngle*1.0 / 360 * 2 * PI));
    moveTo(newX, newY);
    
    Direction newDirection;
    newDirection = (m_posAngle + 180 + 360) % 360;
    setDirection(newDirection);
}

///////////////////////////////////////////////////////////////////////////
//  Spray implementation
///////////////////////////////////////////////////////////////////////////
void Weapon::doSomething()
{
    // Check to see if it’s still alive. If not, it must immediately return.
    if(isDead())
    {
        return;
    }
    
    //Check to see if it overlaps with a damageable object (e.g., a bacterium of any type, goodie, fungus or a dirt pile). To check for overlap, see if the spray has a Euclidean distance of <= SPRITE_RADIUS*2 between itself and the target object. If so, the spray must:
    //a. Tell a single impacted object that it has been damaged with 2 hit points of damage (if the spray overlaps with multiple objects at the same time, it must only damage one of them – which one is left to you to decide. Don’t ask us which object should be chosen.).
    //b. Immediately set its status to not-alive, so the spray goes away immediately.
    //c. Return immediately.
    if (m_world->weaponDamage(getX(), getY(), getDamage()))
    {
        setDead();
        return;
    }
    
    //Otherwise, the spray will move forward in its current direction by SPRITE_RADIUS*2 pixels.
    if (m_travDis >= 0)
    {
        moveAngle(getDirection(), SPRITE_RADIUS*2);
        m_travDis -= SPRITE_RADIUS*2;
    }
    //If the spray has moved a total of 112 pixels after step 3, then it immediately sets its status to not alive (it dissipates) and can cause no more damage.
    if (m_travDis < 0)
    {
        setDead();
    }
}

///////////////////////////////////////////////////////////////////////////
//  Goodie implementation
///////////////////////////////////////////////////////////////////////////

void Goodie::doSomething()
{
    //Goodie::doSomething();
    
    //check to see if it is currently alive. If not, then its doSomething() method must return immediately
    if (isDead())
        return;
    
    //determine if it overlaps with Socrates.
    if (getWorld()->isPlayerNear(getX(),getY(),2*SPRITE_RADIUS))
    {
        //Inform the StudentWorld object that the user is to receive some points.
        getWorld()->increaseScore(getScore());
        //Each type of goodie would do some customized things
        doCustomized();
        //Set its state to dead (so that it will be removed from the game by the StudentWorld object at the end of the current tick).
        setDead();
        //return immediately
        return;
    }
    
    //check to see if its lifetime has expired, and if so, set its state to dead so it disappears from the game.
    passOneTick();
}

///////////////////////////////////////////////////////////////////////////
//  Goodie Specialization
///////////////////////////////////////////////////////////////////////////

void RestoreHealthGoodie::doCustomized()
{
    //Play a sound effect to indicate that Socrates picked up the goodie: SOUND_GOT_GOODIE.
    getWorld()->playSound(SOUND_GOT_GOODIE);
    //Tell the Socrates object to restore its health to full.
    getWorld()->setPlayerHealth(MAX_HEALTH);
}

void FlameThrowerGoodie::doCustomized()
{
    //Play a sound effect to indicate that Socrates picked up the goodie: SOUND_GOT_GOODIE.
    getWorld()->playSound(SOUND_GOT_GOODIE);
    //Tell the Socrates object to add 5 flame thrower charges to its arsenal.
    getWorld()->addPlayerFlames(ADD_FLAMES);
}

void ExtraLifeGoodie::doCustomized()
{
    //Play a sound effect to indicate that Socrates picked up the goodie: SOUND_GOT_GOODIE.
    getWorld()->playSound(SOUND_GOT_GOODIE);
    //Tell the StudentWorld class that the player has earned an extra life by calling StudentWorld::incLives() method (this method is inherited by StudentWorld from GameWorld).
    getWorld()->incLives();
}

void Fungus::doCustomized()
{
    //Tell the Socrates object that it has received 20 points of damage.
    getWorld()->damagePlayer(FUNGUS_DAMAGE);
}

///////////////////////////////////////////////////////////////////////////
//  Pit implementation
///////////////////////////////////////////////////////////////////////////

void Pit::doSomething()
{
    //determine whether or not it has emitted all of its bacteria
    //If it has emitted all of its bacteria
    if(m_sum <= 0)
    {
        //Inform the StudentWorld that it has emitted all of its bacteria
        m_world->decClear();
        //Set its status to not-alive so it disappears
        setDead();
    }
    
    //there is a 1 in 50 chance that during a tick, a given pit will emit a bacterium.
    else if (randInt(0,49) == 0)
    {
        //The pit must randomly pick a type of bacterium to add to the Petri dish (it can only choose from amongst bacteria it hasn’t already run out of)
        int i = randInt(0,2);
        int type;
        if (m_bacteria[i] > 0)
            type = i;
        else if (m_bacteria[(i+1)%3] > 0)
            type = (i+1)%3;
        else
            type = (i+2)%3;
        
        //DO:: It must create a new bacterium object of the appropriate type (regular salmonella, aggressive salmonella or E. coli) with an (x ,y) coordinate that is the same as the pit’s (x ,y) coordinate.
        Bacteria* added = nullptr;
        switch(type)
        {
            case 0:
                added = new RSalmonella(getX(), getY(), m_world);
                break;
            case 1:
                added = new ASalmonella(getX(), getY(), m_world);
                break;
            case 2:
                added = new Ecoli(getX(), getY(), m_world);
                break;
        }
        
        //add to StudentWorld and decrement the type it just emitted
        m_world->addToList(added);
        m_bacteria[type]--;
        m_sum--;
        
        //It (or some other object) must play a sound effect to indicate that the bacterium was just born: SOUND_BACTERIUM_BORN.
        m_world->playSound(SOUND_BACTERIUM_BORN);
    }
    
}


///////////////////////////////////////////////////////////////////////////
//  Bacteria implementation
///////////////////////////////////////////////////////////////////////////

//something in common of the three types of bacteria
void Bacteria::allBacteriaDo()
{
    //Step2/3: check overlap with Socrates
    if(m_world->isPlayerNear(getX(), getY(),2*SPRITE_RADIUS))
    {
        m_world->damagePlayer(getDamage());
    }
    
    else
    {
        //Step3/4: check if it can divide
        if (m_eaten == 3)
        {
            double newX = getX();
            if (newX < VIEW_WIDTH/2)
                newX += SPRITE_WIDTH/2;
            else if (newX > VIEW_WIDTH/2)
                newX -= SPRITE_WIDTH/2;
            double newY = getY();
            if (newY < VIEW_HEIGHT/2)
                newY += SPRITE_WIDTH/2;
            else if (newY > VIEW_HEIGHT/2)
                newY -= SPRITE_WIDTH/2;
            
            //add a new bacteria object at the specified (newx, newy) coordinate.
            Bacteria* added = constructBacteria(newX, newY, m_world);
            m_world->addToList(added);
            m_eaten = 0;
        }
        
        //Step4/5: if it cannot but have something to eat
        else
            m_world->bacteriaEat(getX(), getY(), m_eaten);
    }
}

//something in common of the two types of Salmonella
void Bacteria::SalmonellaMove()
{
    if (m_movePlan > 0)
    {
        m_movePlan--;
        double x, y;
        getPositionInThisDirection(getDirection(), 3, x, y);
        double disToCenter = (x-VIEW_WIDTH/2)*(x-VIEW_WIDTH/2)+(y-VIEW_HEIGHT/2)*(y-VIEW_HEIGHT/2);
        
        //if it's NOT blocked
        if (disToCenter <= VIEW_RADIUS*VIEW_RADIUS && !m_world->bacteriaBlocked(getX(), getY(), x, y))
            moveTo(x,y);
        
        //if it's blocked
        else
        {
            setDirection(randInt(0, 359));
            m_movePlan = 10;
            return;
        }
        return;
    }
    else
    {
        int dir = -999;
        if(!m_world->findPizza(getX(), getY(), dir))
        {
            setDirection(randInt(0, 359));
            m_movePlan = 10;
            return;
        }
        
        else
        {
            double x, y;
            getPositionInThisDirection(dir, 3, x, y);
            if(m_world->bacteriaBlocked(getX(), getY(), x, y))
            {
                setDirection(randInt(0, 359));
                m_movePlan = 10;
                return;
            }
            setDirection(dir);
            moveTo(x,y);
        }
        return;
    }
}

//called when bacterium is damaged, plays the sound and decrease hit points
void Bacteria::takeDamage(int damage)
{
    m_hit -= damage;
    if (m_hit <= 0)
    {
        m_world->playSound(getDieSound());
        m_world->increaseScore(100);
        if(randInt(0,1) > 0)
        {
            Food* addedPizza = new Food(getX(), getY());
            m_world->addToList(addedPizza);
        }
        setDead();
    }
    else
        m_world->playSound(getHurtSound());
}

///////////////////////////////////////////////////////////////////////////
//  Bacteria specialization
///////////////////////////////////////////////////////////////////////////

//ASalmonella chases the player and get blocked
void ASalmonella::chasePlayer(bool& skip)
{
    if (getWorld()->isPlayerNear(getX(), getY(), 72))
    {
        double x, y;
        int dir = getDirection();
        if (getWorld()->isPlayerNear(getX(), getY(), 0))
        {
            skip = true;
            return;
        }
        getWorld()->findPlayer(getX(), getY(), dir);
        getPositionInThisDirection(dir, 3, x, y);
        if (!getWorld()->bacteriaBlocked(getX(), getY(), x, y))
        {
            setDirection(dir);
            moveTo(x,y);
        }
        skip = true;
        return;
    }
    skip = false;
}

void RSalmonella::doSomething()
{
    //Step1: if dead, return
    if(isDead())
        return;
    
    //Step2: check overlap with Socrates
    //Step3: check if it can divide
    //Step4: if it cannot but have something to eat
    allBacteriaDo();
    
    //Step5: if it has a movePlan > 0
    //Step 6: find food to eat
    SalmonellaMove();
}

void ASalmonella::doSomething()
{
    //Step1: if dead, return
    if(isDead())
        return;
    
    //step2: chase the player is it's nearby
    bool skip = false;
    chasePlayer(skip);
    
    //Step3: check overlap with Socrates
    //Step4: check if it can divide
    //Step5: if it cannot but have something to eat
    allBacteriaDo();
    
    //Step6: if it has a movePlan > 0
    //Step7: find food to eat
    if(skip == false)
        SalmonellaMove();
}

///////////////////////////////////////////////////////////////////////////
//  Ecoli implementation
///////////////////////////////////////////////////////////////////////////

void Ecoli::doSomething()
{
    //Step1: if dead, return
    if(isDead())
        return;
    
    //Step2: check overlap with Socrates
    //Step3: check if it can divide
    //Step4: if it cannot but have something to eat
    allBacteriaDo();
    
    //Step5: find and track down player in a smarter way
    bool skip;
    chasePlayer(skip);
    
}

//Ecoli chases the player and changes the direction
void Ecoli::chasePlayer(bool& skip)
{
    if (getWorld()->isPlayerNear(getX(), getY(), 256))
    {
        double x, y;
        int dir = getDirection();
        if (getWorld()->isPlayerNear(getX(), getY(), 0))
            return;
        getWorld()->findPlayer(getX(), getY(), dir);
        for (int i = 0; i < 10; i++)
        {
            getPositionInThisDirection(dir, 2, x, y);
            if (!getWorld()->bacteriaBlocked(getX(), getY(), x, y))
            {
                setDirection(dir);
                moveTo(x,y);
                return;
            }
            else
                dir = (dir + 10) % 360;
        }
    }
}
