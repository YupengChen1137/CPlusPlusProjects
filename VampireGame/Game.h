//
//  Game.hpp
//  project1
//
//  Created by Yupeng Chen on 1/11/20.
//  Copyright © 2020 Yupeng Chen. All rights reserved.
//
#ifndef Game_h
#define Game_h

#include <string>
class Arena;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
};

#endif /* Game_h */

