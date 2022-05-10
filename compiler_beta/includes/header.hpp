#ifndef _HEADER
#define _HEADER

// Includes
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stack> 
using std::string;
using std::endl;
using std::vector;
using std::stack;
using std::cout;

int     isInTable(string& str, vector<string>& table);
void    get_infix();
void    get_infix_expr(string& str, string& function_name);
void    get_symbol_table(string str, vector<string> &symbol_table);
bool    isOperator(char c);
bool    is_number(string& str);
string  infixTOpostfix(std::string args);
string  infixTOprefix(std::string args);
void    postfixToAssembly(string& postfix, vector<string>& table, vector<string>& expr_vec, string& function_name);
void    parse(string& str, vector<string>& vec);

#endif