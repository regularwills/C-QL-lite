#include <iostream>
#include "state_machine_functions.h"

using namespace std;
//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]){
    for (int row = 0; row < MAX_ROWS; row++){
            for (int column = 0; column < MAX_COLUMNS; column++){
                _table[row][column] = -1;
            }
        }
        // ALL table values set to -1 at this point
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state){
    _table[state][0] = 1;       // Marked this row to a success state
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state){
    _table[state][0] = 0;       // Marked this row to a fail state
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state){
    if (_table[state][0] == 1){ return true;}
    else {return false;}
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    while (from <= to){
        _table[row][from] = state;
        from++;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state){
    const char* columns_ptr = columns;
    while (*columns_ptr != '\0' ){
        _table[row][*columns_ptr] = state;
        columns_ptr++;
    }
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state){
    table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]){
    for (int row = 0; row < MAX_ROWS; row++){
        cout << endl; //New Row
        for (int column = 0; column < MAX_COLUMNS; column++){
            cout << _table[row][column];
        }
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos){
    char *s_ptr = s;
    while (*s_ptr != '\0'){
        cout << *s_ptr;
        s_ptr++;
    }      
}

