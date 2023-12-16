#include "stokenize.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];
STokenizer::STokenizer(){
    _pos = 0;
    _done = false;
    make_table(_table);
}
STokenizer::STokenizer(char str[]){
    _pos = 0;
    _done = false;
    strcpy(_buffer, str);
    make_table(_table);
}
bool STokenizer::done(){
   if(_done == true || _pos>strlen(_buffer)){
    return true;
   }
   return false;
}            
bool STokenizer::more(){
    if(_pos<=strlen(_buffer) && _done != true){
        return true;
    }
    else{
        return false;
    }
}            
STokenizer& operator >> (STokenizer& s, Token& t){
    if(s.done()){
        return s;
    }

    string substring;
    int type = 0;

    //Making sure the variables are empty.
    substring.clear();
    t = Token();

    //Grabing token and type.
    s.get_token(type, substring);
    t = Token(substring, type);

    return s;
}
void STokenizer::set_string(char str[]){ 
    _pos = 0;
    _done = false;
    strcpy(_buffer, str);
}
int STokenizer::return_pos(){
    return _pos;
}
void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    
    init_table(_table);
    
    //Altering the code here to define success states and condition.
    
    //Digits
    mark_cells(0, _table, DIGITS, 1);
    mark_cells(1, _table, DIGITS, 1);
    mark_cell(1, _table, '.', 6);               
    mark_cells(6, _table, DIGITS, 7);
    mark_cells(7, _table, DIGITS, 7);
    mark_success(_table, 7);
    mark_success(_table, 1);
    mark_fail (_table, 6);

    //Alphabet
    mark_cells(0, _table, ALPHA, 2);
    mark_cells(2, _table, ALPHA, 2);
    mark_success(_table, 2); 

    //Space
    mark_cells(0, _table,SPACES, 3);
    mark_cells(3, _table,SPACES, 3);
    mark_success(_table, 3);

    //Operators
    mark_cells(0, _table, OPERATORS, 4);
    mark_cells(4, _table, OPERATORS, 4);
    mark_success(_table, 4);

    //Punctuations.
    mark_cells(0, _table, PUNC, 5);
    mark_cells(5, _table, PUNC, 5);
    mark_success(_table, 5);
}
bool STokenizer::get_token(int &start_point, string& token){

    if(strlen(_buffer) == 0 || _buffer[_pos] < 0 || _pos == strlen(_buffer) || _buffer[_pos] == '\0'){
        _pos++;
        _done = true;
        return done();
    }
    
    int begin = _pos;
    int inc = _pos;
    int current_state = 0;

    current_state = _table[current_state][_buffer[_pos]];
    while(current_state != -1 && _buffer[_pos] != '\0'){
        inc++;                                                              
        if(_table[current_state][0] == 1){
            _pos = inc;
            start_point = current_state;
        }
        current_state = _table[current_state][_buffer[inc]];
    }

    //Prints right before _pos because _pos is pointing at the failed character.
    for(int i = begin; i < _pos; i++){ 
        token += _buffer[i];
    }

    return more();
}


//Helper Functions for the Class table
void STokenizer::init_table(int _table[][MAX_COLUMNS]){
    for(int i = 0; i < MAX_ROWS; i++){
        for(int j = 0; j < MAX_COLUMNS; j++){
            if(j == 0){
                _table[i][j] = 0;
            }
            else{
                _table[i][j] = -1;
            }
        }
    }
}
void STokenizer::mark_success(int _table[][MAX_COLUMNS], int row){
    _table[row][0] = 1;
}
void STokenizer::mark_fail(int _table[][MAX_COLUMNS], int row){
    _table[row][0] = -1;
}
bool STokenizer::is_success(int _table[][MAX_COLUMNS], int row){
    return _table[row][0] == 1;
}
void STokenizer::mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    for(int i = from; i <= to; i++){
        _table[row][i] = state;
    }
}
void STokenizer::mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state){
    for(int i = 0; i < strlen(columns); i++){
        _table[row][columns[i]] = state;
    }
}
void STokenizer::mark_cell(int row, int table[][MAX_COLUMNS], int column, int state){
    _table[row][column] = state;
}

//For Debugging
void STokenizer::print_table(int _table[][MAX_COLUMNS]){
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j < MAX_COLUMNS; j++){
            cout << _table[i][j] << " <-" << j << " ";
        }
        cout << endl << endl;
    }
}
void STokenizer::show_string(char s[], int _pos){
    cout << s << endl;
}
