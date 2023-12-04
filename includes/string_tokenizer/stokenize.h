#ifndef STOKENIZE_H
#define STOKENIZE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <cstring>
#include <string.h>

#include "token.h"
#include "tokenizer_constants.h"

using namespace std;

class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);
    
    //set a new string as the input string
    void set_string(char str[]);

    int return_pos();
   
private:
    //Fill all cells of the array with -1
    void init_table(int _table[][MAX_COLUMNS]);

    //Mark this state (row) with a 1 (success)
    void mark_success(int _table[][MAX_COLUMNS], int row);

    //Mark this state (row) with a 0 (fail)
    void mark_fail(int _table[][MAX_COLUMNS], int row);

    //true if state is a success state
    bool is_success(int _table[][MAX_COLUMNS], int row);

    //Mark a range of cells in the array. 
    void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state);

    //Mark columns represented by the string columns[] for this row
    void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state);

    //Mark this row and column
    void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state);

    //This can realistically be used on a small table
    void print_table(int _table[][MAX_COLUMNS]);

    //show string s and mark this position on the string:
    //hello world   pos: 7
    //       ^
    void show_string(char s[], int _pos);

    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int &start_point, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
    bool _done;
};

#endif