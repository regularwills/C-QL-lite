#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <cstring>
#include <string.h>
#include <queue>
#include "../string_tokenizer/stokenize.h"
#include "../stl_utils/vector_utilities.h"
#include "../table/typedefs.h"
#include "../string_tokenizer/token.h"
#include "../maps/map.h"
#include "../maps/multi_map.h"
#include "../state_machine/state_machine_functions.h"

class Parser {
 public:
    Parser();
    Parser(char* s);

    void set_string(char* s);
    int get_column(string& s);
    bool get_parse_tree();
    bool fail(int state);

    MMap<string, string> parse_tree();


 //private:
    MMap<string, string> ptree;
    Map<string, int> map_of_keywords;
    static int adj_table[MAX_ROWS][MAX_COLUMNS];
    vector<string> input_q;
    bool flag;

    void make_adj_table();
    void build_keyword_list();

   //Enumerated words attached to adj_table matrix columns
    enum keywords {
      SYMBOL = 1,
      SELECT = 2,
      MAKE = 3,
      INSERT = 4,
      FROM = 5, 
      STAR = 6,
      FIELDS = 7,
      WHERE = 8,
      VALUES = 9
    };
};

#endif//PARSER_H