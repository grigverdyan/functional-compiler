#include "header.hpp"

int precedence(char c)
{
    if(c == '^')
        return 3;
    else if(c == '*' || c == '/')
        return 2;
    else if(c == '+' || c == '-')
        return 1;
    return -1;
}

string  infixTOpostfix(std::string args)
{
    stack<char> st;
    string postfix;

    for(int i = 0 ; i < args.length() ; ++i) 
    {
        char c = args[i];
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            postfix += c;
        else if(c == '(')
            st.push('(');
        else if(c == ')') 
        {
            postfix += ' ';
            while(st.top() != '(')
            {
                postfix += st.top();
                st.pop();
            }
            st.pop();
        }
        else
        {
            postfix += ' ';
            while(!st.empty() && precedence(args[i]) <= precedence(st.top()))
            {
                postfix += st.top();
                st.pop(); 
            }
            st.push(c);
            postfix += ' ';
        }
    }
 
    while(!st.empty())
    {
        postfix += ' ';
        postfix += st.top();
        st.pop();
    }

    return (postfix);
}

string infixTOprefix(std::string str)
{
    reverse(str.begin(), str.end());
    string prefix;

    for(int i = 0 ; i < str.length() ; i++)
    {
        if(str[i] == '(')
            str[i++] = ')';
        else if(str[i] == ')')
            str[i++] = '(';
    }

    prefix = infixTOpostfix(str);
    reverse(prefix.begin(), prefix.end());
    return prefix;
}
