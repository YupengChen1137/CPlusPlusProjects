//
//  Header.h
//  project1
//
//  Created by Yupeng Chen on 1/11/20.
//  Copyright © 2020 Yupeng Chen. All rights reserved.
#ifndef globals_h
#define globals_h

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////


const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed
const int INITIAL_VAMPIRE_HEALTH = 2;  // initial vampire health
const int POISONED_IDLE_TIME = 1;      // poisoned vampire idles this many turns
                                       //   between moves

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////
class Arena;

int randInt(int lowest, int highest);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
void clearScreen();

#endif /* globals_h */
