/* 
 * File:   Token.cpp
 * Author: John Shelnutt
 * Synopsis: Token class stores information about tokens within an expression. Tokens have a type, value, and a priority in terms of order of operations. 
 */

#include "Token.h"
#include <sstream>
#include <iostream>
using namespace std;

string getStringType(const Token_type t) {
    switch (t) {
        case ID:
            return "ID";
        case INT:
            return "INT";
        case OP:
            return "OP";
        case EQ:
            return "EQ";
        case OpenBrace:
            return "OpenBrace";
        case CloseBrace:
            return "CloseBrace";
        case INVALID:
            return "INVALID";
        default:
            return "/!\\/!\\ type is out of enum range /!\\/!\\";
    }
}

Token::Token() {
    type = INVALID;
    token = "";
    priority = -1;
}

Token::Token(string s) {
    set(s);
}

void Token::set(string s) {
    // valid tokens will have their type changed
    type = INVALID;
    token = s;
    priority = -1;
    
    // Find types if string is 1 character
    if (s.length() == 1) {
        
        if (s[0] == '=') {
            type = EQ;
        }
        else if (s[0] == '+' || s[0] == '-') {
            type = OP;
            priority = 1;
        }
        else if (s[0] == '*' || s[0] == '/' || s[0] == '%') {
            type = OP;
            priority = 2;
        } 
        else if (s[0] == '(') {
            type = OpenBrace;
            priority = 0;
        }
        else if (s[0] == ')') {
            type = CloseBrace;
            priority = 0;
        }
        else if (isalpha(s[0])) {
            type = ID;
        }
        else if (isdigit(s[0]) && s[0] != '0') {
            type = INT;
        }        
    }
    // Determine if identifier, integer, or invalid - non-alphanumeric chars are invalid at this point
    else {
        // Integer token cannot start with 0
        if (s[0] == '0') {return;}
        
        for (int i = 0; i < s.length(); i++) {
            // function is done if a symbol is found
            if (!isalpha(s[i]) && !isdigit(s[i])) {
                return;
            }  
            
            // function is done if the first char is a digit, but not every char is
            if (isdigit(s[0]) && !isdigit(s[i])) {
                return;
            }
                      
        }
        if (isdigit(s[0])) {
            type = INT;
        }
        else {
            type = ID;
        }
    }
}

int Token::value() const {
    if (type == INT) {
        int intVal;
        istringstream(token) >> intVal;
        return intVal;
    }
    else if (type == ID) {
        return -1;
    }
    else {
        return -2;
    }
}

Token_type Token::get_type() const {
    return type;
}

string Token::get_token() const {
    return token;
}

int Token::get_priority() const {
    return priority;
}

void Token::display() const {
    cout << "type     = " << getStringType(type) << endl
         << "token    = " << token << endl
         << "priority = " << priority << endl;    
}
