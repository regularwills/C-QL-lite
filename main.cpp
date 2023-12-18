#include <iostream>
#include <iomanip>
#include "includes/shunting_yard/shuntingyard.h"


using namespace std;

int main(int argv, char** argc) {
    cout << "\n\n"
         << endl;

    vector<string> infix = {"last", "=", "yao", "or", "first", "=", "flo"};
    queue<Token*> postfix_queue;

    infix_to_postfix(infix, postfix_queue);

    while (!postfix_queue.empty()){
        cout << postfix_queue.front()->token_str() << " ";
        postfix_queue.pop();
    }
    
    
  
    return 0;
}