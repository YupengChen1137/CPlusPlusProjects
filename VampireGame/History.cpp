//
//  History.cpp
//  project1
//
//  Created by Yupeng Chen on 1/11/20.
//  Copyright © 2020 Yupeng Chen. All rights reserved.
//

#include "History.h"
#include <iostream>
#include <cstdlib>
using namespace std;

History::History(int nRows, int nCols)
{
    // Record the size of the arena
    m_rows = nRows;
    m_cols = nCols;
    
    // Initialize the record to be all zero's
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_grid[r-1][c-1] = 0;
}

bool History::record(int r, int c)
{
    // Check boundaries
    if (r <= 0  ||  c <= 0  ||  r > m_rows  ||  c > m_cols)
        return false;
    
    // Record the input position
    m_grid[r-1][c-1] ++;
    return true;
}

void History::display() const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill displayGrid with dots (empty) and letters
    for (r = 1; r <= m_rows; r++){
        for (c = 1; c <= m_cols; c++){
            if (m_grid[r-1][c-1] > 0)
            {
                displayGrid[r-1][c-1] = 64 + m_grid[r-1][c-1];
                if (displayGrid[r-1][c-1] > 90)
                    displayGrid[r-1][c-1] = 90;
            }
            else
                displayGrid[r-1][c-1] = '.';
        }
    }
    
    //clear the screen and display the history
    clearScreen();
    
    for (r = 1; r <= m_rows; r++)
    {
        for (c = 1; c <= m_cols; c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
}
