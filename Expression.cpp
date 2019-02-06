/* 
 * File:   Expression.cpp
 * Author: John Shelnutt
 * Synopsis: File to implement expression methods and helper functions to evaluate and parenthesize expressions for the overall calculator
 */

#include "Expression.h"
#include <stack>
#include <iostream>
using namespace std;


bool isSpecialTok(const char c) {
    switch (c) {
        // Operators
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        // Assignment
        case '=':
        // OpenBrace, CloseBrace
        case '(':
        case ')':
            return true;
        default:
            return false;
            
    }
}

string getStringType(const Exp_type t){
    switch (t) {
        case assignment:
            return "assignment";
        case arithmetic:
            return "arithmetic";
        case illegal:
            return "illegal";
        default:
            return "/!\\/!\\ type is out of enum range /!\\/!\\";
    }
}

int evaluate(int a, int b, string op) {
    switch (op[0]) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        case '%':
            return a % b;
        default:
            return -1; // Never makes it here
    }
}

int stringToInt(string s) {
    istringstream stream(s);
    int i;
    stream >> i;
    return i;
}

Expression::Expression() {
    set("");
}

Expression::Expression(const string& s) {
    set(s);
}

void Expression::set(const string& s) {
    original = s;
    valid = false;
    type = illegal;
    if (original == "") {return;}
    
    setTokens();
    setType();
    setPostfix();
    setPrefix();
    setParenthesized();
}

void Expression::display() const {
    cout << "original  = " << original << endl
         << "tokenized = ";
    for (Token t : tokenized) {
        cout << t.get_token() << ",";
    }
    cout << endl 
         << "number of tokens: " << tokenized.size() << endl
         << "postfix   = ";
    for (Token t : postfix) {
        cout << t.get_token() << ",";
    }
    cout << endl
         << "valid     = " << valid << endl
         << "type      = " << getStringType(type) << endl;
}

string Expression::get_original() const {
    return original;
}

vector<Token> Expression::get_tokenized() const {
    return tokenized;
}

vector<Token> Expression::get_postfix() const {
    return postfix;
}

string Expression::get_prefix() const {
    return prefix;
}

string Expression::get_parenthesized() const {
    return parenthesized;
}

string Expression::get_type() const {
    return getStringType(type);
}

/*
 * Function to compute the result of an expression given the variable table
 * Returns the integer result after a PEMDAS order of evaluation on the overall expression
 */
int Expression::get_result(map<string, int>& variables) const {
    if (getStringType(type) == "arithmetic") {
        stack<int> operands;
        int op1, op2;
        for (Token t : postfix) {
            if (t.get_type() == ID) {
                operands.push(variables[t.get_token()]); // Push the corresponding value of the variable, not the variable itself
            } else if (t.get_type() == INT) {
                operands.push(stringToInt(t.get_token()));
            } else if (t.get_type() == OP) {
                op2 = operands.top();
                operands.pop();
                op1 = operands.top();
                operands.pop();
                
                operands.push(evaluate(op1, op2, t.get_token()));
            }
        }
        return operands.top();
    }
    return -1; //Never makes it here, main program won't call this function for non-arithmetic functions
}

/*
 * Function to tokenize the expression for evaluation
 */
void Expression::setTokens() {
    // keeps track of what token we are currently making
    // does not consider if the token is valid or not
    bool inOrdinary = false;
    
    int tokstart = 0;
    int toklength = 1;
    
    for (int i = 0; i < original.length(); i++) {
        // have we found a special token?
        if (isSpecialTok(original[i])) {
            // if we are not already evaluating an INT or ID;
            if (inOrdinary) {
                // add ordinary token
                tokenized.push_back(Token(original.substr(tokstart, toklength)));
            }
            // set variables correctly for special token
            tokstart = i;
            toklength = 1;
            // add special token
            tokenized.push_back(Token(original.substr(tokstart, toklength)));
            // update helper variables
            tokstart = i + 1;
            toklength = 0;
            inOrdinary = false;
        }
        else if (original[i] == ' ') {
            if (inOrdinary) {
                tokenized.push_back(Token(original.substr(tokstart, toklength)));
            }
            tokstart = i + 1;
            toklength = 0;
            inOrdinary = false;
        }
        // letter, number, or symbol
        else {
            if (!inOrdinary) {
                tokstart = i;
                toklength = 1;
                inOrdinary = true;
              // If we are already in an ordinary token, increment the token length
            } else {
                toklength++;
            }
        }
        // Tokenizes the last token, if necessary
        if (i == original.length() - 1 && toklength != 0) {
            tokenized.push_back(Token(original.substr(tokstart, toklength)));
        }
    }
}

// Determines if the given expression is arithmetic, assignment, or invalid
void Expression::setType() {
    enum States {operand, func};
    bool eqtrue = false; // set to true if = is encountered
    States state = operand; // initial state value
    int pcount = 0; // for checking braces
    valid = true; // a field in Expression class
    
    for (Token t : tokenized) {
        if (state == operand) {
            if (t.get_type() == OpenBrace) {pcount++;}
            else if (t.get_type() == INT || t.get_type() == ID) {state = func;}
            else {valid = false; return;}
        } else if (state == func) {
            if (t.get_type() == CloseBrace) {
                pcount--;
                if (pcount < 0) {valid = false; return;}
            } else if (t.get_type() == EQ) {
                eqtrue = true;
                state = operand;
            } else if (t.get_type() == OP) {
                state = operand;
            } else {
                valid = false;
                return;
            }
        } else {
            cout << "state is done and should not have reached this point!" << endl;
        }
    }
    
    if (pcount != 0) {valid = false; return;}
    if (state == operand) {valid = false; return;}
    if (eqtrue) {
        if (tokenized.size() == 3 && tokenized[0].get_type() == ID && tokenized[2].get_type() == INT) {
            type = assignment;
        } else {
            valid = false;
        }
    } else {
        type = arithmetic;
    }
}

// Function to arrange the tokens in postfix order for evaluation
void Expression::setPostfix() {
    if (type != arithmetic) {return;}
    
    stack<Token> ops;
    
    for (Token t : tokenized) {
        if (t.get_type() == ID || t.get_type() == INT) {
            postfix.push_back(t);
        } else if (t.get_type() == OpenBrace) {
            ops.push(t);
        } else if (t.get_type() == CloseBrace) {
            while (ops.size() != 0 && ops.top().get_type() != OpenBrace) {
                postfix.push_back(ops.top());
                ops.pop();
            }
            ops.pop(); // remove OpenBrace from stack 
        } else { // Operator logic here
            while (ops.size() != 0 && ops.top().get_priority() >= t.get_priority()) {
                postfix.push_back(ops.top());
                ops.pop();
            }
            ops.push(t);
        }
    }
    // Flush ops stack after reaching the end of the expression
    while (ops.size() != 0) {
        postfix.push_back(ops.top());
        ops.pop();
    }  
}

// Function to arrange the tokens in prefix notation
void Expression::setPrefix() {
    if (type != arithmetic) {return;}
    
    stack<string> tmp;
    string op1, op2;
    
    for (Token t : postfix) {
        // Push operands to prefix immediately
        if (t.get_type() == ID || t.get_type() == INT) {
            tmp.push(t.get_token());
        // Insert operators before top two operands    
        } else {
            op2 = tmp.top();
            tmp.pop();
            
            op1 = tmp.top();
            tmp.pop();
            
            tmp.push(t.get_token() + " " + op1 + " " + op2);
        }
    }
    prefix = tmp.top();
}

// Function to add parentheses to the expression to indicate the order of operations under PEMDAS
void Expression::setParenthesized() {
    if (type != arithmetic) {return;}
    
    stack<string> tmp;
    string op1;   // operand 1
    string op2;   // operand 2
    for (Token t : postfix) {
        if (t.get_type() == ID || t.get_type() == INT) {
            tmp.push(t.get_token());
        } else if (t.get_type() == OP) {
            op2 = tmp.top();
            tmp.pop();
            
            op1 = tmp.top();
            tmp.pop();
            
            tmp.push("(" + op1 + t.get_token() + op2 + ")");
        }               
    }
    parenthesized = tmp.top();
}

Expression& Expression::operator=(const Expression& e) {
    original = e.original;
    tokenized = e.tokenized;
    postfix = e.postfix;
    prefix = e.prefix;
    parenthesized = e.parenthesized;
    valid = e.valid;
    type = e.type;
    
    return *this;
}