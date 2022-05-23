#ifndef LEXER_H
#define LEXER_H

#include "header.hpp"

class   Lexer {
public:
    int                     m_lineNumber;
    std::queue<std::string> m_parsedCode;

    Lexer(std::string& code);

    void                    getCodeLine();
    void                    codeLineLexemme();
private:
    std::string             m_code;
    std::string             m_expression;
    std::string             m_variable;
    std::list<std::string>  m_postfix;
    std::queue<std::string> m_lexemme;

    bool                    isBracket(const char& symbol);
    bool                    isOperand(const char& operand);
    bool                    isOperator(const char& oper);
    bool                    bracketValidator(const std::string& code);
    bool                    operatorValidator(const std::string& code);
    void                    infixToPostfix();
    void                    setParsedCode();
    void                    setExpression(const std::string& exp);
    std::string             getPostfix();
    int                     getPriority(const char& symbol);
};

#endif // LEXER_H
