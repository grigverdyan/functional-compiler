#ifndef LEXER_H
#define LEXER_H

#include "header.hpp"

// aliases
using QueueString  = std::queue<std::string>;
using ListString   = std::list<std::string>;

class   Lexer {
public:
    int             m_lineNumber;
    QueueString     m_parsedCode;
    
    Lexer(std::string& code);

    void            getCodeLine();
    void            codeLineLexemme();

private:
    std::string     m_code;
    std::string     m_expression;
    std::string     m_variable;
    ListString      m_postfix;
    QueueString     m_lexemme;

    bool            isBracket(const char& symbol);
    bool            isOperand(const char& operand);
    bool            isOperator(const char& oper);
    bool            bracketValidator(const std::string& code);
    bool            operatorValidator(const std::string& code);
    void            infixToPostfix();
    void            setParsedCode();
    void            setExpression(const std::string& exp);
    void            skipIdentifiers(std::string& line);
    std::string     getPostfix();
    int             getPriority(const char& symbol);
};

#endif // LEXER_H
