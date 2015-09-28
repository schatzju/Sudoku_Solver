//  Sudoku.cpp
//  Sudoku_Solver
//
//  Created by Jule Schatz on 5/28/15.
//  Copyright (c) 2015 Jule Schatz. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include "Sudoku.h"

using namespace std;

Sudoku::Group::Cell::Cell(int col_in, int row_in, int squ_in, int val_in): col(col_in), row(row_in), squ(squ_in), val(val_in), size_n(0){
    
    for(int i = 0; i < 9; i++){
        n_pot[i] = 0;
    }
}

//removes potentials from the cell
void Sudoku::Group::Cell::change_n(int n, Group* rows[9], Group* cols[9], Group* squs[9]){
    
    assert(n > 0 && n < 10);
    
    if(val == 0){
        
        int* arr = n_pot;
        int* size = &size_n;
        int index = -1;
        
        //checks if array has n and finds where
        for(int i = 8; i > -1; i--){
            if(arr[i] == n) index = i;
        }

            if(index >= 0){
                arr[index] = arr[*size-1];
                arr[*size-1] = 0;
                (*size)--;
            }
    }
}

Sudoku::Group::Group(): group_size(0){}

void Sudoku::Group::add_cell(Cell* temp){
    group_n[group_size] = temp;
    group_size++;
}

Sudoku::~Sudoku(){
    for(int i = 0; i < 81; i++){
        delete game[i];
    }
    
    for(int k = 0; k < 9; k++){
        delete rows[k];
        delete cols[k];
        delete squs[k];
    }
}

//new stuff is the groups
/*
Sudoku::Sudoku(const Sudoku &other){
    Group* rows[9];
    Group* cols[9];
    Group* squs[9];
}
*/
Sudoku::Sudoku(string file_name){
    
    to_solve = 0;
    
    for(int k = 0; k < 9; k++){
        Group* temp1 = new Group;
        Group* temp2 = new Group;
        Group* temp3 = new Group;
        
        rows[k] = temp1;
        cols[k] = temp2;
        squs[k] = temp3;
    }
    
    ifstream filestream;
    filestream.open(file_name);
    if(!filestream.is_open()){
        cout << "FAIL" << endl;
        exit(1);
    }
    
    int num_in;
    int row_n = 0;
    int index = 0;
    int index_save = index;
    
    while(filestream >> num_in){
        
        int val, squ;
        val = num_in;
        if(val == 0) to_solve++;
        
        //seting col
        while(index > 8){
            index = index - 9;
            ++row_n;
        }
        
        if(index <= 2 && row_n <= 2) squ = 0;
        else if( index <= 5 && row_n <= 2) squ = 1;
        else if( index < 9 && row_n <= 2) squ = 2;
        
        else if( index <= 2 && row_n <= 5) squ = 3;
        else if( index <= 5 && row_n <= 5) squ = 4;
        else if( index < 9 && row_n <= 5) squ = 5;
        
        else if( index <= 2 && row_n < 9) squ = 6;
        else if( index <= 5 && row_n < 9) squ = 7;
        else if( index < 9 && row_n < 9) squ = 8;
        
        Group::Cell* temp = new Group::Cell(index, row_n, squ, val);
        
        index = index_save;
        
        game[index++] = temp;
        
        index_save++;
        
        row_n = 0;
    }
    
    filestream.close();
    
    int r = 0, c = 0, s = 0;
    
    for(int i = 0; i < 81; i++){
        
        cols[c]->add_cell(game[i]);
        rows[r]->add_cell(game[i]);
        squs[s]->add_cell(game[i]);
        
        
        if((i+1)%3 == 0) ++s;
        if((i+1)%9 == 0){
            ++r;
            if(r < 3) s = 0;
            else if(r < 6) s = 3;
            else s = 6;
            c = 0;
        }
        else ++c;
        
    }
    bool insert;
    int row;
    int col;
    int squ;
   
    //adds potentials
    for(int h = 0; h < 81; h++){
        if(game[h]->val == 0){
            row = game[h]->row;
            col = game[h]->col;
            squ = game[h]->squ;
        
            for(int k = 1; k < 10; k++){
                insert = true;
               
                for(int i = 0; i < 9; i++){
                    if(rows[row]->group_n[i]->val == k){
                        insert = false;
                    }
                    if(cols[col]->group_n[i]->val == k){
                        insert = false;
                    }
                    if(squs[squ]->group_n[i]->val == k){
                        insert = false;
                    }
                }
                if(insert == true){
                    game[h]->n_pot[game[h]->size_n] = k;
                    game[h]->size_n++;
                }
                
                
            }
        }
    }
    
}// end constructor

void Sudoku::print(){
    for(int i = 0; i < 81; i++){
        cout << game[i]->val << " ";
        if((i+1)%3 == 0) cout << " ";
        if((i+1)%9 == 0) cout << endl;
        if((i+1)%27 == 0) cout << endl;
    }
}

void Sudoku::insert_check(Group::Cell* c, int n){
    Group* rr = rows[c->row];
    Group* cc = cols[c->col];
    Group* ss = squs[c->squ];
    
    //Goes through all cells that are effected by that cell and removes the
    //potential number that was just added to that cell c
    for(int i = 0; i < 9; i++){
        rr->group_n[i]->change_n(n, rows, cols, squs);
        cc->group_n[i]->change_n(n, rows, cols, squs);
        ss->group_n[i]->change_n(n, rows, cols, squs);
    }
    c->size_n = 0;
    to_solve--;
}

bool Sudoku::only_cell(int ind, bool change, string type, bool change2){
    
    int size;
    Group** arr;
    int* po;
    
    if(type == "col") arr = cols;
    else if(type == "row") arr = rows;
    else arr = squs;
    
    if(change == false) return change2;
    else{
        for(int i = 0; i < 9; i++){
            Group::Cell* temp = arr[ind]->group_n[i];
            size = temp->size_n;
            po = temp->n_pot;
            
            if(temp->val == 0){
                if(size == 1){
                    temp->val = po[0];
                    insert_check(temp, temp->val);
                    only_cell(ind, true, type, true);
                }
            }
        }
        
        return only_cell(ind, false, type, change2);
    }
}

bool Sudoku::only_num(int ind, string type){
    
    bool only = true;
    int num, k;
    
    int size, size2;
    Group** arr;
    int* po;
    int* po2;
    
    if(type == "col") arr = cols;
    else if(type == "row") arr = rows;
    else arr = squs;
    
    
    //goes through each cell in that row
    for(int h = 0; h < 9; ++h){
        
        Group::Cell* temp = arr[ind]->group_n[h];
        size = temp->size_n;
        po = temp->n_pot;
        
        //goes through all potential elements of the row_n
        for(int i = 0; i < size; i++){
            num = po[i];
            
            //for each potential element goes through all other elements until it finds it
            k = 0;
            only = true;
            while(only && k < 9){
                Group::Cell* temp2 = arr[ind]->group_n[k];
                size2 = temp2->size_n;
                po2 = temp2->n_pot;
                
                
                //if cell isn't cell you are checking, and it's value is a 0
                if(k != h && temp2->val == 0){
                    //run through all of that cells potential looking for match
                    for(int t = 0; t < size2; t++){
                        if(po2[t] == num) only = false;
                    }
                }
                k++;
                
            }
            if(only == true){
                temp->val = num;
                insert_check(temp, temp->val);
                return true;
            }
        }
    }
    return false;
}

bool Sudoku::num_pot(Group::Cell* temp_cell, int num){
    for(int i = 0; i < temp_cell->size_n; i++){
        if(temp_cell->n_pot[i] == num) return true;
    }
    return false;
}

void Sudoku::narrow_row(int ind){
     //go through square, goes through three rows then three cols, and if a number can go in at least two spaces, check if it's anywhere else. if it isn't get rid of that number as a potential through that row or col
    
    bool whole_squ = true;
    int counter = 0;
    
    //goes through each row and col in that square
    for(int i = 0; i < 8; i = i + 3){
    
        Group::Cell* cur_cell = squs[ind]->group_n[i];
        Group::Cell* nxt_cell = squs[ind]->group_n[i+1];
        Group::Cell* nxt_nxt_cell = squs[ind]->group_n[i+2];

        for(int k = 1; k < 10; k++){
            counter = 0;
            nxt_cell = squs[ind]->group_n[i+1];
            //if(the number k is a potential for two or more in the first row
            if(((num_pot(cur_cell, k) && (num_pot(nxt_cell, k) || num_pot(nxt_nxt_cell, k))) || (num_pot(nxt_cell, k) && num_pot(nxt_nxt_cell, k)))){
                    
                while(whole_squ && counter < 9){
                    if(counter != i && counter != i+1 && counter != i+2){
                        nxt_cell = squs[ind]->group_n[counter];
                        if(num_pot(nxt_cell, k)) whole_squ = false;
                    }
                    ++counter;
                }
            }
            else whole_squ = false;
            
            if(whole_squ){
                //loop through row skipping the square we are on
                int row_index = squs[ind]->group_n[i]->row;
                for(int w = 0; w < 9; w++){
                    if(rows[row_index]->group_n[w]->squ != ind){
                        rows[row_index]->group_n[w]->change_n(k, rows, cols, squs);
                    }
                    
                }
            }
            whole_squ = true;
        }
    }
}

void Sudoku::narrow_col(int ind){
    //go through square, goes through three rows then three cols, and if a number can go in at least two spaces, check if it's anywhere else. if it isn't get rid of that number as a potential through that row or col
    
    bool whole_squ = true;
    int counter = 0;
    
    //goes through each row and col in that square.. 0 3 6 | 1 4 7 |2 5 8
    for(int i = 0; i < 3; i++){
        
        Group::Cell* cur_cell = squs[ind]->group_n[i];
        Group::Cell* nxt_cell = squs[ind]->group_n[i+3];
        Group::Cell* nxt_nxt_cell = squs[ind]->group_n[i+6];
        
        for(int k = 1; k < 10; k++){
            counter = 0;
            nxt_cell = squs[ind]->group_n[i+3];
            //if(the number k is a potential for two or more in the first row
            if(((num_pot(cur_cell, k) && (num_pot(nxt_cell, k) || num_pot(nxt_nxt_cell, k))) || (num_pot(nxt_cell, k) && num_pot(nxt_nxt_cell, k)))){
                
                while(whole_squ && counter < 9){
                    if(counter != i && counter != i+3 && counter != i+6){
                        nxt_cell = squs[ind]->group_n[counter];
                        if(num_pot(nxt_cell, k)) whole_squ = false;
                    }
                    ++counter;
                }
            }
            else whole_squ = false;
            
            if(whole_squ){
                //loop through col skipping the square we are on
                int col_index = squs[ind]->group_n[i]->col;
                for(int w = 0; w < 9; w++){
                    if(cols[col_index]->group_n[w]->squ != ind){
                        cols[col_index]->group_n[w]->change_n(k, rows, cols, squs);
                    }
                    
                }
            }
            whole_squ = true;
        }
    }
}
 //go through row, goes through three squares. if a number can go in at least two paces in that square but no where else in that row. get rid of that number as a potential through that square
void Sudoku::narrow_rs(int ind){
    bool whole_squ = true;
    int counter = 0;
    int start;
    
    if(ind < 3) start = 0;
    else if(ind < 6) start = 3;
    else start = 6;
    
    //goes through each square in the row  // 0 1 2 | 3 4 5 | 6 7 8
    for(int i = 0; i < 7; i = i + 3){
    
        
        Group::Cell* cur_cell = rows[ind]->group_n[i];
        Group::Cell* nxt_cell = rows[ind]->group_n[i+1];
        Group::Cell* nxt_nxt_cell = rows[ind]->group_n[i+2];
        
        for(int k = 1; k < 10; k++){
            counter = 0;
            nxt_cell = rows[ind]->group_n[i+1];
            
            //if(the number k is a potential for two or more in the square
            
            if(((num_pot(cur_cell, k) && (num_pot(nxt_cell, k) || num_pot(nxt_nxt_cell, k))) || (num_pot(nxt_cell, k) && num_pot(nxt_nxt_cell, k)))){
                
                while(whole_squ && counter < 9){
                    if(counter != i && counter != i+1 && counter != i+2){
                        nxt_cell = rows[ind]->group_n[counter];
                        if(num_pot(nxt_cell, k)) whole_squ = false;
                    }
                    ++counter;
                }
            }
        
            
            else whole_squ = false;
            
            if(whole_squ){
                //loop through square skipping the row we are on
                //int row_index = rows[ind]->group_n[i]->col;
                int squ_index = start + (i/3);
                for(int w = 0; w < 9; w++){
                    if(squs[squ_index]->group_n[w]->row != ind){
                        squs[squ_index]->group_n[w]->change_n(k, rows, cols, squs);
                    }
                    
                }
            }
            whole_squ = true;
        }
    }
}
void Sudoku::narrow_cs(int ind){
    bool whole_squ = true;
    int counter = 0;
    int start;
    
    if(ind < 3) start = 0;
    else if(ind < 6) start = 1;
    else start = 2;
    
    //goes through each square in the col  // 0 1 2 | 3 4 5 | 6 7 8
    for(int i = 0; i < 7; i = i + 3){
        
        
        Group::Cell* cur_cell = cols[ind]->group_n[i];
        Group::Cell* nxt_cell = cols[ind]->group_n[i+1];
        Group::Cell* nxt_nxt_cell = cols[ind]->group_n[i+2];
        
        for(int k = 1; k < 10; k++){
            counter = 0;
            nxt_cell = cols[ind]->group_n[i+1];
            
            //if(the number k is a potential for two or more in the square
            
            if(((num_pot(cur_cell, k) && (num_pot(nxt_cell, k) || num_pot(nxt_nxt_cell, k))) || (num_pot(nxt_cell, k) && num_pot(nxt_nxt_cell, k)))){
                
                while(whole_squ && counter < 9){
                    if(counter != i && counter != i+1 && counter != i+2){
                        nxt_cell = cols[ind]->group_n[counter];
                        if(num_pot(nxt_cell, k)) whole_squ = false;
                    }
                    ++counter;
                }
            }
            
            
            else whole_squ = false;
            
            if(whole_squ){
                //loop through square skipping the row we are on
                //int row_index = rows[ind]->group_n[i]->col;
                int squ_index = start + (i);
                for(int w = 0; w < 9; w++){
                    if(squs[squ_index]->group_n[w]->col != ind){
                        squs[squ_index]->group_n[w]->change_n(k, rows, cols, squs);
                    }
                    
                }
            }
            whole_squ = true;
        }
    }
}




//Logic: if a number can only go in two places in a row
//check all other rows below if the same thing exists

void Sudoku::square(int row){
    //number can only go in two places, save col numbers
    //check other rows if that number can only go in those cols as well then you have a square

    int numb[3][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, //count of potentials of 1-9
        {0, 0, 0, 0, 0, 0, 0, 0, 0}, //the col of the first occurance
        {0, 0, 0, 0, 0, 0, 0, 0, 0}  //the col of the second occurance
    };
    Group::Cell* temp;
    
    int* place;
    
    
    //goes through row
    for(int i = 0; i < 9; i++){
        
        temp = rows[row]->group_n[i];
        
        //if cell is empty
        if(temp->val == 0){
            
            //check all potentials that could go there
            for(int k = 0; k < temp->size_n; k++){
                place = &(numb[0][temp->n_pot[k] - 1]);
                (*place)++;
                
                //adds col index
                if(*place < 3){
                numb[*place][temp->n_pot[k] -1] = i;
                }
            }
        }
    }
    
    //for all potentials that appear exactly twice check rows for the same
    int pot;
    int col1;
    int col2;
    
    bool is_square = true;
    
        
    //goes through all potentials with two occurances
    for(int y = 0; y < 9; y++){
            
        //if there are exactly two occurances
        if(numb[0][y] == 2){
            pot = y + 1;
            col1 = numb[1][y];
            col2 = numb[2][y];
                
            //goes through all rows below it
            for(int q = row + 2; q < 9; q++){
                
                //goes through row
                for(int i = 0; i < 9; i++){
                    
                    temp = rows[q]->group_n[i];
                    
                    //if cell is empty
                    if(temp->val == 0){
                        
                        //check all potentials that could go there
                        for(int k = 0; k < temp->size_n; k++){
                            
                            //if potential is the same as one we are looking for
                            if(temp->n_pot[k] == pot)
                            {
                                //if it is in one of the cols
                                if(i != col1 && i != col2){
                                    is_square = false;
                                }
                
                            }
                        }
                    }
                }
                
                if(is_square == true){
                    temp = rows[q]->group_n[col1];
                    is_square = false;
                    
                    if(temp->val == 0){
                        
                        //check all potentials that could go there
                        for(int k = 0; k < temp->size_n; k++){
                            
                            //if potential is the same as one we are looking for
                            if(temp->n_pot[k] == pot){
                                is_square = true;
                            }
                        }
                    }
            
                    temp = rows[q]->group_n[col2];
                    is_square = false;
                    
                    if(temp->val == 0){
                        
                        //check all potentials that could go there
                        for(int k = 0; k < temp->size_n; k++){
                            
                            //if potential is the same as one we are looking for
                            if(temp->n_pot[k] == pot){
                                is_square = true;
                            }
                        }
                    }
                }
                
                if(is_square == true){
                    delete_square(row, col1, col2, q, pot);
                }
            }
        }
    }
}

void Sudoku::delete_square(int row1, int col1, int col2, int row2, int num){
    
    for(int i = col1 + 1; i < col2; i++){
        rows[row1]->group_n[i]->change_n(num, rows, cols, squs);
    }
    
    for(int k = row1 + 1; k < row2; k++){
        cols[col2]->group_n[k]->change_n(num, rows, cols, squs);
    }
    
    for(int w = col2 - 1; w > col1; w--){
        rows[row2]->group_n[w]->change_n(num, rows, cols, squs);
    }
    
    for(int y = row2 - 1; y > row1; y--){
        cols[col1]->group_n[y]->change_n(num, rows, cols, squs);
    }
}


//Checks if there is a contradiction, in potentials, and value
bool Sudoku::con(){

    int value;
    int column;
    int square;
    int rowl;
    int size_potential;
    
    
    for(int i = 0; i < 81; i++){
        
        value = game[i]->val;
        column = game[i]->col;
        square = game[i]->squ;
        rowl = game[i]->row;
        size_potential = game[i]->size_n;
        
        //checks for actual contradiction
        if(value != 0){
            for(int k = 0; k < 9; k++){
                //go through cell's col
                if(cols[column]->group_n[k]->row != rowl && cols[column]->group_n[k]->val == value){
                    cout << "c index " << i << " " << value << endl;
                    return true;
                }
                
                if(rows[rowl]->group_n[k]->col != column && rows[rowl]->group_n[k]->val == value){
                    cout << "r index " << i << " " << value << endl;
                    return true;
                }
                
                if((squs[square]->group_n[k]->row != rowl && squs[square]->group_n[k]->col != column) && squs[square]->group_n[k]->val == value){
                    cout << "s index " << i << " " << value << endl;
                    return true;
                }
            }
        }
        
        //checks for contradiction with potentials
        else{
            //checks if no number can go there anymore
            if(size_potential == 0){
                cout << "potentail is zero " << i << endl;
                return true;
            }
        }
    }
    
    bool foundc = false;
    bool foundr = false;
    bool founds = false;
    Group::Cell* cur_c;
    Group::Cell* cur_r;
    Group::Cell* cur_s;
    
    for(int j = 0; j < 9; j++){
        //checks in each row and col and square if each potentail is accounted for
        //goes through each row, col, and square
            
            //checks for each value
        for(int h = 1; h < 10; h++){
            foundc = false;
            founds = false;
            foundr = false;
            
            for(int q = 0; q < 9; q++){
                
                cur_r = rows[j]->group_n[q];
                cur_c = cols[j]->group_n[q];
                cur_s = squs[j]->group_n[q];
                
                if(cur_r->val == h) foundr = true;
                else if(cur_r->val == 0){
                    for(int v = 0; v < cur_r->size_n; v++){
                        if(cur_r->n_pot[v] == h) foundr = true;
                    }
                }
                
                if(cur_s->val == h) founds = true;
                else if(cur_s->val == 0){
                    for(int v = 0; v < cur_s->size_n; v++){
                        if(cur_s->n_pot[v] == h) founds = true;
                    }
                }
                
                if(cur_c->val == h) foundc = true;
                else if(cur_c->val == 0){
                    for(int v = 0; v < cur_c->size_n; v++){
                        if(cur_c->n_pot[v] == h) foundc = true;
                    }
                }
            
                }
            if(foundr == false || foundc == false || founds == false){
                cout << "not found " << h << " " << j << endl;
                return true;
            }
        }
        
    }
     return false;
}


/*
//go through cells with val 0 and plug in a potential and then try to solve, if there is a contradiction get rid of that num as a potential. if its solved then you are done. if it's not solved go back to original.
void Sudoku::carl_stradegy_helper(){
    
    for(int i = 0; i < 81; i++){
        if(game[i]->val == 0){
            for(int v = 0; v < game[i]->size_n; v++){
                insert_check(game[i], game[i]->n_pot[v]);
            }
        }
    
    }
}

*/

/*
void Sudoku::print_sec(int ind, string type){
    Group** arr;
    
    if(type == "col") arr = cols;
    else if(type == "row") arr = rows;
    else arr = squs;
    
    for(int i = 0; i < 9; i++){
        cout << arr[ind]->group_n[i]->val;
        cout << " ";
    }
    cout << endl;
}
*/
void Sudoku::loop_all(bool change){
    
    if(change == false) return;
    
    change = false;
        
        for(int i = 0; i < 9; i++){
            if(only_num(i, "row")) change = true;
            if(only_num(i, "col")) change = true;
            if(only_num(i, "squ")) change = true;
            
            if(only_cell(i, true, "row", false)) change = true;
            if(only_cell(i, true, "col", false)) change = true;
            if(only_cell(i, true, "squ", false)) change = true;
        }
    
    loop_all(change);
}

void Sudoku::solve(){
    
    loop_all(true);
    
    int num = 0;
  
    while(to_solve > 0 && num < 3){
            for(int i = 0; i < 9; i++){
                narrow_row(i);
                narrow_col(i);
                narrow_rs(i);
                narrow_cs(i);
            }
        
        loop_all(true);
    
        if(to_solve > 0){
            for(int i = 0; i < 9; i++){
                square(i);
            }
            
        loop_all(true);
        
        }
       num++;
    
     }
    
    if(to_solve > 0){
        
        if(con()) cout << "Carl: oh no I seem to have made a mistake" << endl;
       // else{
       //     carl_stradegy();
       // }
    }

    if(to_solve > 0){
            cout << "Carl: Sorry, but I couldn't solve your sudoku... squares left " << to_solve << endl;
            print();
    }
    
    else{
        cout << "Carl: Got it!" << endl;
        print();
    }
}




