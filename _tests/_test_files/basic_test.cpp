#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
using namespace std;

/*
=====  Basic test for the Database here  =====

*/

bool basic_test(bool debug = true){
  if (debug){
    cout << "\nbasic test...\n" << endl;
  }
  return true;
}

TEST(BASIC_TEST, BasicTest) {
  
  //EXPECT_EQ(1, <individual test functions are called here>);

  EXPECT_EQ(1, basic_test(false));
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running basic_test.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

