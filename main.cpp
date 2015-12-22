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
    string filename;
    cout << "Hello, I'm Carl;\nI solve sudoku puzzles and let you know what level they are. The levels range from 1 to 3, easiest to hardest; \nPlease type in the name of the file of the sudoku you wish for me to solve; \nTo end the program simply type end;\n" << endl;
    
    cout << "Carl: I'm ready for the first sudoku;" << endl;
    while(cin >> filename){
        if(filename == "end"){
            cout << "Carl: Bye!;" << endl;
            break;
        }
        else{
            Sudoku game(filename);
            game.solve();
        }
        cout << "Carl: I'm ready for a sudoku to solve;" << endl;
    }
}


