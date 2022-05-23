#ifndef PARSER_H
#define PARSER_H

#include "header.hpp"
#include "user_defined_types.hpp"

class   Parser {
public:
    Parser();

    void    lexemmeToTokenString(std::string lexemme, int line);
    void    parseLineCode(int lineNumber);
    void    generateOutputCode();


private:
    std::stringstream               m_dataSection;
    std::stringstream               m_codeSection;
    std::stringstream               m_t_varStream;
    std::stringstream               m_t_functionStream;
    std::stack<std::string>         m_langSpecifiers;
    std::map<std::string,E_Type>    m_varNameType;
    int                             m_parsedCodeLineNumber;

    bool                            isRegistor(const std::string& var);
    bool                            isNumber(std::string& s);
    bool                            isOperator(std::string& obj);
    bool                            isVariable(std::string& var);
    bool                            isFunction(std::string& obj);
    E_Reg                           findEmptyReg(E_Type type);
    E_Ar                            findEmptyAr();
    E_Type                          getType(int& number);
    E_Type                          getRegType(int regNum);
    const std::string               getTypename(E_Type type);
    void                            changeRegState(E_Reg); 
    void                            changeArState(E_Ar);
    void                            generateCode(E_Reg, E_Reg, std::string&, int, std::stringstream&, std::stringstream&, std::stack<E_Reg>&, std::stack<std::string>&);
    void                            generateFunctionCall(std::string& temp, std::stringstream& tempCodeStream);
    void                            getDataType(int arg, std::stringstream& t_dataStream);
    void                            updateRegAr();
    void                            generateCodeSectionCode();

    std::map<E_Reg, bool>       m_RegNameState
    {
        std::make_pair(E_Reg::R0,  true),
        std::make_pair(E_Reg::R1,  false),
        std::make_pair(E_Reg::R2,  false),
        std::make_pair(E_Reg::R3,  false),
        std::make_pair(E_Reg::R4,  false),
        std::make_pair(E_Reg::R5,  false),
        std::make_pair(E_Reg::R6,  false),
        std::make_pair(E_Reg::R7,  false),
        std::make_pair(E_Reg::R8,  false),
        std::make_pair(E_Reg::R9,  false),
        std::make_pair(E_Reg::R10, false),
        std::make_pair(E_Reg::R11, false),
        std::make_pair(E_Reg::R12, false),
        std::make_pair(E_Reg::R13, false),
        std::make_pair(E_Reg::R14, false),
        std::make_pair(E_Reg::R15, false),
        std::make_pair(E_Reg::R16, false),
        std::make_pair(E_Reg::R17, false),
        std::make_pair(E_Reg::R18, false),
        std::make_pair(E_Reg::R19, false),
        std::make_pair(E_Reg::R20, false),
        std::make_pair(E_Reg::R21, false),
        std::make_pair(E_Reg::R22, false),
        std::make_pair(E_Reg::R23, false),
        std::make_pair(E_Reg::R24, false),
        std::make_pair(E_Reg::R25, false),
        std::make_pair(E_Reg::R26, false),
        std::make_pair(E_Reg::R27, false),
        std::make_pair(E_Reg::R28, false),
        std::make_pair(E_Reg::R29, false),
        std::make_pair(E_Reg::R30, false),
        std::make_pair(E_Reg::R31, false),
        std::make_pair(E_Reg::undefind, false),
    };
       
    std::map<E_Ar, bool> m_addressRegistorState
    {
        std::make_pair(E_Ar::A2,false),
        std::make_pair(E_Ar::A3,false),
        std::make_pair(E_Ar::A4,false),
        std::make_pair(E_Ar::A5,false),
        std::make_pair(E_Ar::A6,false),
        std::make_pair(E_Ar::A7,false),
        std::make_pair(E_Ar::SP,true),
        std::make_pair(E_Ar::SF,true)
    };

};

#endif  // PARSER_H
