//  Suduko Solver
//
//  Created by Jule Schatz on 5/6/15.
//  Copyright (c) 2015 Jule Schatz. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cassert>
#include <unistd.h>
#include "Sudoku.h"

using namespace std;

int main() {

    Sudoku game("sudoku.txt");
    game.solve();
    
    Sudoku game2("sudoku2.txt");
    game2.solve();
    
    Sudoku game3("sudoku3.txt");
    game3.solve();
    
    Sudoku game4("sudoku4.txt");
    game4.solve();
    
   // Sudoku game5("sudoku5.txt");
    //game5.solve();
    
    Sudoku game6("sudoku6.txt");
    game6.solve();
    
    Sudoku game7("sudoku7.txt");
    game7.solve();
    
    Sudoku game8("sudoku8.txt");
    game8.solve();
    
    Sudoku game9("sudoku9.txt");
    game9.solve();

    //can't solve
    Sudoku game10("sudoku11.txt");
    game10.solve();
  
    Sudoku game11("sudoku12.txt");
    game11.solve();
    
 
   char * dir = getcwd(NULL, 0); // Platform-dependent, see reference link below
    printf("Current dir: %s", dir);
    
     
}

