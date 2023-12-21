#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <set>
#include <vector>
#include <iomanip>
using namespace std;


//=====  Basic test for the Database here  =====
//Files we are testing:
#include "../../includes/sql/sql.h"
//==============================================

using namespace std;

const vector<string> command_list = {

/*00*/     "make table employee fields  last,       first,         dep,      salary, year",
/*01*/     "insert into employee values Blow,       Joe,           CS,       100000, 2018",
/*02*/     "insert into employee values Blow,       JoAnn,         Physics,  200000, 2016",
/*03*/     "insert into employee values Johnson,    Jack,          HR,       150000, 2014",
/*04*/     "insert into employee values Johnson,    \"Jimmy\",     Chemistry,140000, 2018",

/*05*/     "make table student fields  fname,       lname,    major,    age",
/*06*/     "insert into student values Flo,            Yao, 	Art, 	20",
/*07*/     "insert into student values Bo, 		     Yang, 	CS, 		28",
/*08*/     "insert into student values \"Sammuel L.\", Jackson, 	CS, 		40",
/*09*/     "insert into student values \"Billy\",	     Jackson, 	Math,	27",
/*10*/     "insert into student values \"Mary Ann\",   Davis,	Math,	30",

/*11*/     "select * from employee",
/*12*/     "select last, first, age from employee",
/*13*/     "select last from employee",
/*14*/     "select * from employee where last = Johnson",
/*15*/     "select * from employee where last=Blow and major=\"JoAnn\"",

/*16*/     "select * from student",
/*17*/     "select * from student where (major=CS or major=Art)",
/*18*/     "select * from student where lname>J",
/*19*/     "select * from student where lname>J and (major=CS or major=Art)"

};

const int MAKE_TABLE_COMMANDS = 11;
const int SELECT_COMMANDS = 20;

bool sql_basic(bool debug = true)
{
     SQL sql;
     Table t;
     cout << ">" << command_list[0] << endl;
     sql.command(command_list[0]);
     cout << "basic_test: table created." << endl<<endl;

     for (int i = 1; i < MAKE_TABLE_COMMANDS; i++)
     {
          cout << ">" << command_list[i] << endl;
          sql.command(command_list[i]);
     }

     cout << endl
          << endl;
     for (int i = MAKE_TABLE_COMMANDS; i < command_list.size(); i++)
     {
          cout << "\n>" << command_list[i] << endl;
          if (debug)
               cout<< sql.command(command_list[i])<<endl;
          else
               t = sql.command(command_list[i]);
          cout << "basic_test: records selected: "<<sql.select_recnos() << endl;
     }

     cout << "----- END TEST --------" << endl;
     return true;
}

// ==============================
// global BAD!
bool debug = true;
// ==============================

TEST(SQL_BASIC, SQLBasic) {
  bool success = sql_basic(debug);
  EXPECT_EQ(success, true);
}

int main(int argc, char **argv) {
  if (argc > 1)
  {
    debug = !strcmp(argv[1], "debug");
  }

  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running basic_test.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}
