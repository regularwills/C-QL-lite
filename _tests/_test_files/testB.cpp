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

// ------------------- Btree Tests -------------------
bool basic_test(){
  int a[] = {43, 20, 96, 100, 69};
  BTree<int> bt(a, 5);
  cout << "Here is the tree: " << endl
      << bt << endl;

  cout <<"in order: "<< bt.in_order() << endl;
  cout << "adding 10, 50, 200: " << endl;
  bt.insert(10);
  bt.insert(50);
  bt.insert(200);
  cout << "in order " << bt.in_order() << endl;
  cout << "Here is the tree: " << endl
       << bt << endl;
  cout << "bt.contains(20): " << boolalpha << bt.contains(20) << endl;
  cout << "bt.contains(0): " << boolalpha << bt.contains(0) << endl;
  cout << "bt.find(20): " << *(bt.find(20)) << endl;
  cout << "bt.find(0): " << bt.find(0) << endl;
  
  
  cout << "\n\nexcess + grow: " << endl;

  bt.insert(12);
  cout << "adding 12: " << endl;
  cout << "Here is the tree: " << endl
       << bt << endl;
  cout << "in order " << bt.in_order() << endl;
  bt.insert(300);
  cout << "added 300: " << endl;
  cout << "Here is the tree: " << endl
       << bt << endl;
  cout << "in order " << bt.in_order() << endl;
  bt.insert(55);
  cout << "added 55: " << endl;
  cout << "Here is the tree: " << endl
       << bt << endl;
  cout << "in order " << bt.in_order() << endl;


  // cout << "\n\nremove test: " << endl;

  // int b[] = {43, 20, 96, 10, 50, 200, 12, 300, 55, 100, 69};
  // for (int i = 0; i < 11; i++){
  //   cout << "\n removing: [" << b[i] << "]" << endl;
  //   bt.remove(b[i]);
  //   cout << bt << endl;
  //   cout <<"in order: "<< bt.in_order() << endl;
  //   cout << "\nbt.empty()?" <<boolalpha<<bt.empty() << endl;
  // }


  // cout << "\nremove on empty:" << endl;
  // bt.remove(666);
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
