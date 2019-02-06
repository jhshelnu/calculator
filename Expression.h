/* 
 * File:   Expression.h
 * Author: John Shelnutt
 * Synopsis: Header file describing the structure of expressions, which are the statements that can be evaluated or handled by the calculator
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "Token.h"

using namespace std;

enum Exp_type {assignment, arithmetic, illegal};

class Expression {
public:
    Expression();
    Expression(const string& s);
    void set(const string& s);
    void display() const;
    string get_original() const;
    vector<Token> get_tokenized() const;
    vector<Token> get_postfix() const;
    string get_prefix() const;
    string get_parenthesized() const;
    string get_type() const;
    int get_result(map<string, int>& variables) const;
    Expression& operator=(const Expression& e);
private:
    string original;
    vector<Token> tokenized;
    vector<Token> postfix;
    string prefix;
    string parenthesized;
    bool valid;
    Exp_type type;
    
    // Helper functions
    void setTokens();
    void setType();
    void setPostfix();
    void setPrefix();
    void setParenthesized();
};
#endif /* EXPRESSION_H */