#include "token.h"

Token::Token(){
    _token.clear();
    _type = 0;
    _presedence = 0;
    
}
Token::Token(string str, int type){
    _token.clear();
    _token = str;
    _type = type;  
}

Token::Token(string str, int type, int presedence){
    _token.clear();
    _token = str;
    _type = type;
    _presedence = presedence;
}

int Token::type() const{
    return _type;
}

string Token::type_string() const{   
    string s;
    switch(_type){
        case TOKEN_NUMBER:
            s = "NUMBER";
            break;
        case 7:
            s = "DOUBLE";
            break;
        case TOKEN_ALPHA:
            s = "ALPHA";
            break;
        case TOKEN_SPACE:
            s = "SPACE";
            break;
        case TOKEN_OPERATOR:
            s = "OPERATOR";
            break;
        case TOKEN_PUNC:
            s = "PUNC";
            break;
        case TOKEN_END:
            s = "END"; 
            break;
        default:
            s = "UNKNOWN";
            break;
    }

    return s;
} 

string Token::token_str() const{
    return _token;
}

int Token::get_presedence() const{
    return _presedence;
}

void Token::set_presedence(int p){
    _presedence = p;
}

ostream& operator <<(ostream& outs, const Token& t){
    if(t._token.size() > 0){
        outs << '|'  << t._token << '|';
    }
    return outs;
}
