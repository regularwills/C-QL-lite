#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>

#include "../../includes/string_tokenizer/stokenize.h"
#include "../../includes/bplustree/bplustree.h"

using namespace std;

/*
  UNIT TESTS for all Database components (tables, tokenizer, parser, btree, etc..)
  -Tokenizer
  -Btree
*/

bool test_stub(bool debug = false)
{
  if (debug)
  {
    cout << "testB:: test-sub() entering test_sub" << endl;
  }
  return true;
}

TEST(TEST_STUB, TestStub)
{
  EXPECT_EQ(1, test_stub(false));
}

//-------------------- Tokenizer Tests --------------------


void test_s_tokenize_simple()
{
  char s[] =
      // "\0";
      //"Pain and suffering";
      // "";
      // "3.14....................";
      " \"So, it was the night of october 17th. pi was still 3.14. sigh! 2.\"";
  // "2";
  //  char s[] = "it";
  cout << "\n\ntokenizing: [" << s << "]\n"
       << endl;
  STokenizer stk(s);
  Token t;
  stk >> t;
  while (stk.more())
  {
    // process token here...
    cout << setw(10) << t.type_string() << setw(10) << t << endl;

    t = Token();
    stk >> t;
  }
}

bool test_tokenize(bool debug = false)
{
  test_s_tokenize_simple();
  return true;
}

// ------------------- BPlustree Tests -------------------
bool basic_test(){


  return true;
}


TEST(TEST_BTREE, TestBTreeInsert) {
  bool success = basic_test();
  EXPECT_EQ(success, true);
}


TEST(TEST_TOKENIZER, TestTokenize)
{
  bool success = test_tokenize(true);
  EXPECT_EQ(success, true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "\n\n----------running testB.cpp---------\n\n"
            << std::endl;
  return RUN_ALL_TESTS();
}
