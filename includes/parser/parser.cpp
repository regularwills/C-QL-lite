#include "parser.h"
#include <cstring>
#include <algorithm> 
#include <cassert>

// #include <bits/stdc++.h> when running on my machine
// uncomment to push

using namespace std;

int Parser::adj_table[MAX_ROWS][MAX_COLUMNS];

//Custom function to take inputs and align them with keywords map indexes
string to_lower(string &s){
    transform(s.begin(), s.end(), s.begin(),[](unsigned char c){ return tolower(c); });
    return s;
}
Parser::Parser(){
    make_adj_table();
    build_keyword_list();
}

//GTG
Parser::Parser(char* s){
    //Clear vector of tokens and inialize token list
    // Build class adjcency matrix and the map of keywords and enum values
    make_adj_table();
    build_keyword_list();
    set_string(s);
}

//GTG
void Parser::make_adj_table(){
    //Set all columns and rows to fail state -1
    init_table(adj_table);

    //Commands states
    mark_cell(0, adj_table, SELECT, 1);
    mark_cell(0, adj_table, MAKE, 7);

    mark_cell(0, adj_table, INSERT, 13);
    mark_cell(13, adj_table, SYMBOL, 14);
    mark_cell(14, adj_table, VALUES, 15);
    mark_cell(15, adj_table, SYMBOL, 16);
    mark_cell(16, adj_table, SYMBOL, 16);
    
    //Tables states
    mark_cell(1, adj_table, SYMBOL, 2);
    mark_cell(1, adj_table, STAR, 2);
    mark_cell(2, adj_table, SYMBOL, 2);
    mark_cell(2, adj_table, FROM, 3);
    mark_cell(3, adj_table, SYMBOL, 4);
    mark_cell(4, adj_table, WHERE, 5);
    mark_cell(5, adj_table, SYMBOL, 6);
    mark_cell(6, adj_table, SYMBOL, 6);
    mark_cell(7, adj_table,  SYMBOL, 8);

    //Field/ Values states
    mark_cell(8, adj_table, FIELDS, 9);
    mark_cell(8, adj_table, VALUES, 11);

    //Conditions
    mark_cell(9, adj_table, SYMBOL, 10);
    mark_cell(10, adj_table, SYMBOL, 10);
    mark_cell(11, adj_table, SYMBOL, 12);
    mark_cell(12, adj_table, SYMBOL, 12);

    //States 4,6,10,12,16 are all succesfull end states on the table
    mark_success(adj_table, 4);
    mark_success(adj_table, 6);
    mark_success(adj_table, 10);
    mark_success(adj_table, 12);
    mark_success(adj_table, 16);
}


//GTG
//Grabs a series of token string to tokenize individually and adds them 
// to the class input queue of tokens
void Parser::set_string(char* s){
    input_q.clear();
    STokenizer stk;
    stk.set_string(s);
    Token t;

    while(stk.more()){
        stk>>t;
        //Adding all the tokens except for spaces
        if (t.type() != TOKEN_SPACE && t.token_str() != ""){
            if (t.token_str() != "table" && t.token_str() != "into" && t.token_str() != ","){
                    input_q.push_back(t.token_str());
            }
        }
    }  
   
    get_parse_tree();
}

//GTG
//Gets a token and converts it to your enum class to use for state machine
int Parser::get_column(string& token){
    //When the token is not a SQL command, defualt to symbol state
    if (!(map_of_keywords).contains(to_lower(token))){
        return SYMBOL;
    }
    //Otherwise, the token is a valid SQL keyword, grab that state
    else {
        //Return the tokens enum value from map to the state machineg
        return map_of_keywords[to_lower(token)];
    }
}

// //From the input queue, jump through various states in the adjacency table
// // initalize the ptree with keys and values according to their states
 bool Parser::get_parse_tree(){
    int current_state = 0;
    ptree.clear();
    assert(ptree.empty());

    //???? Hard coded values: error when calling ptree["...."] += saying it dones't contain that value
    ptree.insert("command", "");
    ptree.insert("fields", "");
    ptree.insert("table", "");
    
    //Iterate through all the tokens in the token queue and jump from state to state
    // dont initalize from, field, and value (don't tell us anything)
    for (int i = 0; i < input_q.size(); i++){
        //cout << ptree << endl;
        string yes = "yes";
        current_state = adj_table[current_state][get_column(input_q[i])];

        switch (current_state){
            case 1:{
                // cout << "case 1:" << endl;
                ptree["command"] += input_q[i];
                break;
            }
            case 2:{
                // cout << "case 2:" << endl;
                ptree["fields"] += input_q[i];
                break;
            }
            case 3:{
                // cout << "case 3:" << endl;
                break;
            }
            case 4:{
                // cout << "case 4:" << endl;
                if(!input_q.empty())
                    ptree.insert("condition", "");
                ptree["table"] += input_q[i];
                break; 
            }
            case 5:{
                // cout << "case 5:" << endl;
                ptree["where"] += yes;
                break;   
            }
            case 6:{
                //cout << "case 6:" << endl;
                ptree["condition"] += input_q[i];
                break;
            }
            case 7:{
                //cout << "case 7:" << endl;
                ptree["command"] += input_q[i];
                break;
            }
            case 8:{
                //cout << "case 8:" << endl;
                ptree["table"] += input_q[i];
                break;
            }
            case 9:{
                //cout << "state 9" << endl;
                break;
            }
            case 10:{
                //cout << "case 10:" << endl;
                ptree["fields"] += input_q[i];
                break;
            }
            case 11:{
                //cout << "state: 11 " << endl;
                break;
            }
            case 12:{
                //cout << "case 12:" << endl;
                ptree["values"] += input_q[i];
                break;
            }

            case 13:{
                //cout << "case 13:" << endl;
                ptree.insert("values", "");
                ptree["command"] += input_q[i];
                break;
            }
            case 14:{
                //cout << "case 14:" << endl;
                ptree["table"]+= input_q[i];
                break;
            }        
            case 15:{
                //cout << "case 15:" << endl;
                break;
            }
            case 16:{
                //cout << "case 16:" << endl;
                ptree["values"] += input_q[i];
                break;
            }
        }
    }
    return fail(current_state);
}

//GTG
//Building keyword map with pairs of SQL commands and number values
void Parser::build_keyword_list(){
    //These VALUES MUST be in sync with the private enum values of Parser class
    //Commands "Select..make..insert.."
    map_of_keywords.insert("select", 2);
    map_of_keywords.insert("make", 3);
    map_of_keywords.insert("insert", 4);

    //Tables 
    map_of_keywords.insert("from", 5);

    //All fields "*" or multiple fields
    map_of_keywords.insert("*", 6);
    map_of_keywords.insert("fields", 7);

    //Condition
    map_of_keywords.insert("where", 8);

    //Values
    map_of_keywords.insert("values", 9);
}

//GTG
bool Parser::fail(int state){
    // Evaluating the current success state in the adjacency table
    // Avoids initalzing the parser with bad data at a failed state
    return adj_table[state][0] == -1; 

}

//GTG
// Return the class member ptree (multimap of string and longs)
MMap<string, string> Parser::parse_tree(){
    return ptree;
}

    