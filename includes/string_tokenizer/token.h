#ifndef TOKEN_H
#define TOKEN_H

#include "tokenizer_constants.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
using namespace std;

class Token
{
public:
    Token();
    Token(string str, int type);
    friend ostream &operator<<(ostream &outs, const Token &t);
    int type() const;
    string type_string() const;
    string token_str() const;

    int get_presedence() const;
    void set_presedence(int p);

private:
    string _token;
    int _presedence;
    int _type;
};

#endif // TOKEN_H