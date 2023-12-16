#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include <stack>
#include <queue>
#include "../token_evals/token_children.h"
#include "../string_tokenizer/tokenizer_constants.h"


void infix_to_postfix(vector<string>& infix_expression, queue <Token*>& postfix_queue);




#endif//SHUNTINGYARD_H