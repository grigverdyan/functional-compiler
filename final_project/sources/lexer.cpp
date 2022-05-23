#include "../includes/lexer.hpp"
#include "../includes/user_defined_types.hpp"
#include "../includes/error_message.hpp"

Lexer::Lexer(std::string& code)
{
    m_code = code;
}

void    Lexer::getCodeLine()
{
    const char* delim = ";";
    char*       t_code = const_cast<char*>(m_code.c_str());
    char*       t_str;

    t_str = strtok(t_code, delim);
    while (t_str != NULL) {
        std::string t_obj;
        t_obj += t_str;
        t_obj += ";";
        m_lexemme.push(t_obj);
        t_str = strtok(NULL, delim);
    }
}

void    Lexer::setExpression(const std::string& exp)
{
    m_postfix.clear();
    m_variable.clear();
    m_expression = exp;
    ++m_lineNumber;

    if (!bracketValidator(m_expression)) {
        throw ErrorMessage("\t[Lexer Error]: Invalid bracket in line ", m_lineNumber);
    }
    if (!operatorValidator(m_expression)) {
        throw ErrorMessage("\t[Lexer Error]: Invalid operator in line ", m_lineNumber);
    }

    infixToPostfix();
}

void    Lexer::codeLineLexemme()
{
    getCodeLine();
    while(!m_lexemme.empty()) {
        setExpression(m_lexemme.front());
        m_lexemme.pop();
        setParsedCode(); 
    }
}

void    Lexer::setParsedCode()
{
    m_parsedCode.push(getPostfix());
}

std::string Lexer::getPostfix()
{
    std::string postfix = m_variable + " ";

    for (auto token : m_postfix) {
        postfix += token;
        postfix += " ";
    }
    postfix += E_Operators::equal;

    return postfix;
}

int Lexer::getPriority(const char& symbol)
{
    int priority = -1;
    switch (symbol) {
        case E_Operators::plus:
            priority = 2;
            break;
        case E_Operators::minus:
            priority = 2;
            break;
        case E_Operators::multiply:
            priority = 3;
            break;
        case E_Operators::division:
            priority = 3;
            break;
        case E_Operators::remainder:
            priority = 3;
            break;
        case E_Operators::equal:
            priority = 4;
            break;
        default:
            break;
    }

    return priority;
}

bool    Lexer::isBracket(const char& symbol)
{
    return (symbol == E_Bracket::left || E_Bracket::right) ? true : false;
}

bool    Lexer::isOperand(const char& operand)
{
    return isalnum(operand);
}

bool    Lexer::isOperator(const char& oper)
{
    bool    returnType = false;
    switch(oper) {
        case E_Operators::plus:
            returnType = true;
            break;
        case E_Operators::minus:
            returnType = true;
            break;
        case E_Operators::division:
            returnType = true;
            break;
        case E_Operators::remainder:
            returnType = true;
            break;
        case E_Operators::multiply:
            returnType = true;
            break;
        case E_Operators::equal:
            returnType = true;
            break;
        default:
            break;
    }

    return returnType;
}

bool    Lexer::bracketValidator(const std::string& code)
{
    std::stack<char>    st_bracket;
    for (auto i = 0; i < code.length(); ++i) {
        if (code[i] == E_Bracket::right && st_bracket.empty()) {
            return false;
        } else if (code[i] == E_Bracket::right && !st_bracket.empty()) {
            st_bracket.pop();
        } else if (code[i] == E_Bracket::left) {
            st_bracket.push(code[i]);
        } else if ((i != 0 && code[i - 1] == E_Bracket::left && isOperator(code[i]))
               || (i != 0 && code[i] == E_Bracket::right && isOperator(code[i - 1]))) {
            return false;
        }
    }

    return st_bracket.empty() ? true : false;
}

bool    Lexer::operatorValidator(const std::string& code)
{
    for (auto i = 0; i < code.length() - 1; ++i) {
        if (isOperator(code[i]) && isOperator(code[i + 1])) {
            return false;
        }
    }

    return true;
}

void    Lexer::infixToPostfix()
{
    std::stack<char>    st_operation;
    int count = 0;
    while (m_expression[count] != E_Operators::equal) {
        m_variable += m_expression[count];
        ++count;
    }
    ++count;

    for (auto i = count; i < m_expression.length() - 1; ++i) {
        if (isOperand(m_expression[i])) {
            std::string operand;
            auto j = i;
            operand += m_expression[j];
            j++;
            while (j < m_expression.length() && (isOperand(m_expression[j]) || m_expression[j] == '.')) {
                operand += m_expression[j];
                j++;
                i++;
            }
            m_postfix.push_back(operand);
        } else if (m_expression[i] == E_Bracket::left &&  m_expression[i + 1] == E_Bracket::right && !m_postfix.empty()) {
            std::string t_opeand = m_postfix.back();
            m_postfix.pop_back();
            t_opeand += "()";
            m_postfix.push_back(t_opeand);
            ++i;
        } else if (isOperator(m_expression[i])) {
            while (!st_operation.empty() && st_operation.top() != E_Bracket::left
                   && getPriority(m_expression[i]) < getPriority(st_operation.top()))
            {
                m_postfix.push_back(std::string(1, st_operation.top()));
                st_operation.pop();
            }
            st_operation.push(m_expression[i]);
        } else if (m_expression[i] ==E_Bracket::right) {
            while (!st_operation.empty() && st_operation.top() != E_Bracket::left) {
                m_postfix.push_back(std::string(1, st_operation.top()));
                st_operation.pop();
            }
            st_operation.pop();
        } else if (m_expression[i] == E_Bracket::left) {
            st_operation.push(m_expression[i]);
        }
    }
    while (!st_operation.empty()) {
        if (st_operation.top() != E_Bracket::left) {
            m_postfix.push_back(std::string(1, st_operation.top()));
            st_operation.pop();
        }
    }
}
