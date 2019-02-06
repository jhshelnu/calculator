/* 
 * File:   homework5.cpp
 * Author: John Shelnutt
 * Synopsis: Calculator program to parse expressions and perform a handful of functions like parenthesizing statements and evaluating them * 
 */

#include "Token.h"
#include "Expression.h"
#include <stack>
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

map<string, int> variables; // variable look up table

void updateLookupTable(const Expression& e) {
    // expression e is of the form "a = b", tokenized array is {a, =, b}, where a is the variable name and b is the variable value
    string variableName = e.get_tokenized()[0].get_token();
    int variableValue;
    istringstream stream(e.get_tokenized()[2].get_token());
    stream >> variableValue;
    
    variables.insert(pair<string,int>(variableName, variableValue));
}

// helper function for debugging - prints all the stored variable names and values
void printLookupTable() {
    cout << "contents of the variable look up table: " << endl;
    for (map<string, int>::iterator it = variables.begin(); it != variables.end(); it++) {
        cout << it->first << " => " << it->second << endl;
    }
}

bool containsUndefinedVariable(Expression e) {

    // Look at every token - if you find a variable and it's not in the look up table, the statement cannot be evaluated
    for (Token t : e.get_tokenized()) {
        if (t.get_type() == ID && variables.find(t.get_token()) == variables.end()) {
            return true;
        }
    }
    
    return false;
}

// Function to get valid action input from user
// input MUST be of length 1 and character entered must be in the set of valid options
char getValidAction() {
    string input = "";
    string validCharSet = "=><fFqQcCsS";
    
    cout << "action: ";
    cin  >> input;
    
    while (input.length() > 1 || validCharSet.find(input[0]) == string::npos) {
        cout << "Wrong input for the action! Please type one of =, <, >, f(F), q(Q), c(C), s(S)" << endl;
        cout << "action: ";
        cin  >> input;
    }
    
    return input[0];
}

void printResult(const vector<Expression>& expSequence) {
    for (Expression e : expSequence) {
        if (e.get_type() != "arithmetic" || containsUndefinedVariable(e)) {
            cout << "cannot evaluate " << e.get_original() << endl;
        } else {
            cout << e.get_original() << " = " << e.get_result(variables) << endl;
        }  
    }
}

void printPrefix(const vector<Expression>& expSequence) {
    for (Expression e : expSequence) {
        if (e.get_type() == "arithmetic") {
            cout << "prefix of " << e.get_original() << " is: " << e.get_prefix() << endl;           
        } else {
            cout << e.get_original() << " is not arithmetic and has no prefix" << endl;
        }
    }
}

void printPostfix(const vector<Expression>& expSequence) {
    for (Expression e : expSequence) {
        if (e.get_type() == "arithmetic") {
            cout << "postfix of " << e.get_original() << " is: ";
            for (Token t : e.get_postfix()) {
                cout << t.get_token() << ' ';
            }
            cout << endl;            
        } else {
            cout << e.get_original() << " is not arithmetic and has no postfix" << endl;
        }
    }
}

void printParenthesized(const vector<Expression>& expSequence) {
    for (Expression e : expSequence) {
        if (e.get_type() == "arithmetic") {
            cout << "fully parenthesizing " << e.get_original() << " is: " << e.get_parenthesized() << endl;           
        } else {
            cout << e.get_original() << " cannot be parenthesized" << endl;
        }
    }
}

// Breaks up input into sequence of expressions, then adds assignment values to variable look up table
void addExpressions(vector<Expression>& expressions, const string& s) {
    Expression e;
    int nextExpBreak = s.find(';');
    
    // Base case: If input does not contain a semicolon (or it is at the end), assume one statement and push whole string
    if (nextExpBreak == string::npos) {
        e = Expression(s);
        expressions.push_back(e);
    } else if (nextExpBreak == s.length() - 1) {
        e = Expression(s.substr(0, s.length() - 1));
        expressions.push_back(e);
    // Recursive case: If semicolon was found and it is NOT the last character, then there must be another statement
    //                 Recurse with the remainder of the string starting right after that semicolon position
    } else {
        e = Expression(s.substr(0, nextExpBreak));
        expressions.push_back(e);
        addExpressions(expressions, s.substr(nextExpBreak + 1));
    }
    
    // Add assignment value to look up table (if expression is an assignment statement)
    if (e.get_type() == "assignment") {
        updateLookupTable(e);
    }
}

int main() {
    vector<Expression> expSequence;
    string input;
    char action;
    
    cout << "=== expression evaluation program starts ===" << endl;
    
    cout << "input: ";
    cin  >> input;
    addExpressions(expSequence, input);
    
    do {
        action = getValidAction();

        // Handle action inputs here
        if (action == '=') {
            printResult(expSequence);                
        } else if (action == '>') {
            printPrefix(expSequence);     
        } else if (action == '<') {
            printPostfix(expSequence);
        } else if (action == 'f' || action == 'F') {
            printParenthesized(expSequence);
        } else if (action == 's' || action == 'S') {
            variables.clear();
            cout << "input: ";
            cin  >> input;
            expSequence.clear();
            addExpressions(expSequence, input);
        } else if (action == 'c' || action == 'C') {
            cout << "input: ";
            cin  >> input;
            addExpressions(expSequence, input);
        }
        
//        printLookupTable(); // Print the contents of the variable map for testing purposes
        
    } while (action != 'q' && action != 'Q');
    
    return 0;
}











