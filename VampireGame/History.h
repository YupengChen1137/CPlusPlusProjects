//
//  History.hpp
//  project1
//
//  Created by Yupeng Chen on 1/11/20.
//  Copyright © 2020 Yupeng Chen. All rights reserved.
//
#ifndef History_h
#define History_h

#include "globals.h"

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
};

#endif /* History_h */
