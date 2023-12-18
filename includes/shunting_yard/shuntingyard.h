#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include <stack>
#include <queue>
#include "../string_tokenizer/stokenize.h"


void infix_to_postfix(vector<string>& infix_expression, queue <Token*>& postfix_queue);




#endif//SHUNTINGYARD_H