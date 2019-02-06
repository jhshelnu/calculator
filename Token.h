/* 
 * File:   Token.h
 * Author: John Shelnutt
 * Synopsis: Header file for a token, which is the fundamental building block of expressions in the larger calculator program.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

enum Token_type {ID, INT, OP, EQ, OpenBrace, CloseBrace, INVALID};

class Token {
public:
    Token();
    Token(string s);
    void set(string s);
    void display() const;
    int value() const;
    Token_type get_type() const;
    string get_token() const;
    int get_priority() const;
private:
    Token_type type = ID;
    string token;
    int priority;
};


#endif /* TOKEN_H */

