#include "header.hpp"

void get_infix()
{
    std::fstream indata;
    indata.open("input.txt");
    string infix;
    if (!indata)
    {
        std::cerr << "ERROR: File coud not be opened." << endl;
        exit(1);
    }
    
    string function_name;
    size_t i = 0;
    string postfix;
    string prefix;
    vector<string> symbol_table;
    vector<string> expr_vec;
    while (getline(indata, infix))
    {
        get_infix_expr(infix, function_name);
        get_symbol_table(infix, symbol_table);
        postfix = infixTOpostfix(infix);
        prefix = infixTOprefix(infix);
        parse(postfix, expr_vec);
        postfix.erase(remove(postfix.begin(), postfix.end(), ' '), postfix.end());
        postfix.resize(postfix.length()-count(postfix.begin(), postfix.end(),' '));
        postfixToAssembly(postfix, symbol_table, expr_vec, function_name);
        symbol_table.clear();
        expr_vec.clear();
    }
    indata.close();
}

void    get_infix_expr(string& str, string& function_name)
{
    size_t i;

    i = 0;
    while (str[i] != '=')
        ++i;
    
    ++i;
    function_name = str.string::substr(0, i - 1);
    str = str.string::substr(i, str.length() - i);
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.resize(str.length()-count(str.begin(), str.end(),' '));
}

void    get_symbol_table(string str, vector<string> &symbol_table)
{
    str = str + ")";
    string  temp;
    for(size_t i = 0; i < str.length(); i++)
    {
        if (isOperator(str[i]) || str[i] == '(' || str[i] == ')')
        {
            if (temp[0] && !(is_number(temp)))
                symbol_table.push_back(temp);
            temp = "";
            continue;
        }
        temp.push_back(str[i]);
    }
}

bool    isOperator(char c)
{
    if (c == '-' || c == '+' || c == '*' || c == '/' || c == '=')
        return true;
    return false;
}

bool    is_number(string& str)
{
    size_t i = 0;
    
    while (str[i])  
    {
        if (!isdigit(str[i])) 
            return false; 
        i++;
    }
            
    return true;
}

void    parse(string& str, vector<string>& vec)
{
    size_t i = 0;
    string temp;
    
    while (str[i] == ' ')
        ++i;
    while (str[i])
    {
        temp = "";
        if (str[i] != ' ')
        {
            while (str[i] != ' ' && str[i])
            {
                temp += str[i];
                ++i;
            }
            vec.push_back(temp);
        }
        else
            ++i;
    }
}
