//
//  Arena.hpp
//  project1
//
//  Created by Yupeng Chen on 1/11/20.
//  Copyright © 2020 Yupeng Chen. All rights reserved.
//
#ifndef Arena_h
#define Arena_h

#include <string>
#include "globals.h"
#include "History.h" // the declaration of the member History needs this

class Player;
class Vampire;
class History;

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int      rows() const;
    int      cols() const;
    Player*  player() const;
    int      vampireCount() const;
    int      getCellStatus(int r, int c) const;
    int      numberOfVampiresAt(int r, int c) const;
    void     display(std::string msg) const;
    History& history(); // a public function to access its history

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addVampire(int r, int c);
    bool addPlayer(int r, int c);
    void moveVampires();

  private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Vampire* m_vampires[MAXVAMPIRES];
    int      m_nVampires;
    int      m_turns;
    History  m_history; // data member: a History object
    

      // Helper functions
    void checkPos(int r, int c, std::string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

#endif /* Arena_h */
