#ifndef SQL_H
#define SQL_H

#include <cassert>
#include <cstring>
#include "../table/table.h"
#include "../parser/parser.h"

class SQL{
public:
    SQL();
    Table command(const string input);
    vector <long> select_recnos();

private:
    Table table;
    Parser p;
    mmap_ss ptree;

};


#endif//SQL_H