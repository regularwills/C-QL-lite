#ifndef TABLE_H
#define TABLE_H

#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include "typedefs.h"
#include "shuntingyard.h"
#include "../stl_utils/vector_utilities.h"
#include "../binary_files/utilities.h"
#include "../binary_files/file_record.h"
#include "../token/token_children.h"


#include <stack>
#include <queue>
#include "../string_tokenizer/stokenize.h"

// using namespace std;

//File name will be the same name as the table name
class Table {
 public: 
    Table();
    Table(const string &table_name);
    Table(const string& name, 
          const vector<string>& field_names);

    friend ostream& operator<<(ostream& outs, const Table& print_me);

    void insert_into(vectorstr& record_info);

    Table& select(const vectorstr& fields, vectorstr& infix_conditions);
    Table& select(const vectorstr& fields, queuetokens& postfix_queue);
    Table& select(const vectorstr& fields, const string& field, 
                  const string& symbol, const string& condition);
    Table& select_all();
    
    vectorlong select_recnos();
    void set_fields(const vector<string>& field_names);
    void reindex();

    vectorlong get_record_nums(const vectorstr field_name, const string field, 
                                   const string condition, const string value);
 
 //private: 
      string _name;
      map_sl field_map;
      vectorstr _field_names;
      vectorlong used_records;
      vector <mmap_sl> _indices;
      
      long _serial_number;
      long _last_record_number;
      bool _empty;
};


void infix_to_postfix(vector<string>& infix_expression, queue <Token*>& postfix_queue);

#endif//TABLE_H