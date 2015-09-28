#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

//9 groups for each of the 9 columns, 9 rows, and 9 squares in a Sudoku game
class Sudoku{
private:
    
    //Representation of 9 Cells grouped together
    class Group{
        friend Sudoku;
    private:
        
        //Representation of each 81 Cells of a Sudoku board
        class Cell{
            friend Sudoku;
        private:
            
            //Which column, row, square, and value the Cell has
            int col, row, squ, val;
            
            //The numbers 1-9 needed in the cell
            int n_pot[9];
            
            //Size of col_n, row_n, and squ_n arrays
            int size_n;
            
        public:
            Cell(int col_in, int row_in, int squ_in, int val_in);
            
            //Checks to see if n is already in pot_n,
            //If it isn't and bool add is true it adds it
            //If it is and bool add is false it removes it
            //Otherwise does nothing
            void change_n(int n, Group* rows[9], Group* cols[9], Group* squs[9]);
            
        };//end Cell
        
        Cell* group_n[9];
        int group_size;
        
    public:
        
        Group();
        
        void add_cell(Cell* temp);
        
    };//end Group
    
    Group* rows[9];
    Group* cols[9];
    Group* squs[9];
    
    //needed to initialize the game
    Group::Cell* game[81];
    
    int to_solve;
    
public:
    
    ~Sudoku();
    
    Sudoku(const Sudoku &other);
    
    Sudoku(string file_name);
    
    void print();
    
    void print_sec(int ind, string type);
    
    //if an int n is needed for a col and it can only go two or three spots in the same square that n cannot go anywhere else in that square
    
    //Used after n has been inserted to Cell c
    //removes n as a possibility for all cells in
    //the same row, colomn, and square and Cell c.
    void insert_check(Group::Cell* c, int n);
    
    //Recursive fnc that inserts a number(1-9) if it is
    //the only posibility for that Cell
    bool only_cell(int ind, bool change, string type, bool change2);
    
    //Inserts a number(1-9) if that number can
    //only go in that cell
    bool only_num(int ind, string type);
    
    Group::Cell* nxt_squ(int index, int cont);
    
    bool num_pot(Group::Cell* temp_cell, int num);
    
    //if a number in a square can only go in a certian row/col it cannot go in the row/col in a different square
    
    //go through square, goes through three rows then three cols, and if a number can go in at least two spaces, check if it's anywhere else. if it isn't get rid of that number as a potential through that row or col
    void narrow_row(int ind);
    
    void narrow_col(int ind);
    
    //go through row, goes through three squares. if a number can go in at least two paces in that square but no wehre else in that row. get rid of that number as a potential through that square
    void narrow_rs(int ind);
    //go through col, goes through three squares. if a number can go in at least two places in that square but no where else in that col, get rid of that number as a potential through that square
    
    void narrow_cs(int ind);
    
    void delete_square(int row1, int col1, int col2, int row2, int num);
    
    void square_helper(int place, int consec, int num, int row, int col, int pl1, int pl2, int pl3, int row_s, int org_col);
    
    void square(int row);
    
    void loop_all(bool change);
    
    bool con();
    
    void carl_stradegy();
    
    void solve();
    
    //make fnc to check if there is a contradiciton
    //plug in potential, try to solve, if there is a contradiction get rid of it as a potential, otherwise do nothing
    
    
};//end Sudoku
