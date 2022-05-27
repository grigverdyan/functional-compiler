#include "../includes/parser.hpp"
#include "../includes/user_defined_types.hpp"
#include "../includes/error_message.hpp"
#include "../includes/IO_files.hpp"

Parser::Parser()
{
    m_dataSection << "\n.data\n";
    m_codeSection << "\n.code\n";
}

void  Parser::generateOutputCode()
{
	generateCodeSectionCode();
	IO_Files::outWriteFileStream << m_dataSection.str() << std::endl << 
				  m_codeSection.str() << std::endl;
}

void    Parser::lexemmeToTokenString(std::string lexemme, int line)
{
    const char* first = strchr(lexemme.c_str(), '=');
    const char* last = strrchr(lexemme.c_str(), '=');

    if (first != last) {
        throw ErrorMessage("\t[Parser ERROR]: Put ';' in line ", line);
        exit(7);
    }
    char* lexerStr = const_cast<char*>(lexemme.c_str());
    char* t_str = strtok(lexerStr, " ");
    while (t_str != NULL) {
        std::string obj;
        obj += t_str;
        m_langSpecifiers.push(obj);
        t_str = strtok(NULL, " ");
    }
}

bool    Parser::isNumber(std::string& s)
{
    int i = 0;
    do {
        if (!isdigit(s[i])) {
            return false;
        }
        ++i;
    } while (i < s.size() - 1);

    return true;
}

bool    Parser::isRegistor(const std::string& var)
{
    std::string obj;
    if (var.length() == 3) {
        obj += var[1];
        obj += var[2];
    } else if (var.length() == 2) {
        obj += var[1];
    }

    if ((var.length() == 3 || var.length() == 2 || var.length() == 1)
        && (var[0] == 'R' || var[0] == 'A') && isNumber(obj)) 
    {
        return true;
    }
    return false;
}

bool    Parser::isOperator(std::string& obj)
{
    if (*(obj.c_str()) == E_Operators::plus)
        return true;
    else  if (*(obj.c_str()) == E_Operators::minus)
        return true;
    else  if (*(obj.c_str()) == E_Operators::division)
        return true;
    else if (*(obj.c_str()) == E_Operators::remainder)
        return true;
    else  if (*(obj.c_str()) == E_Operators::multiply)
        return true;
    else  if (*(obj.c_str()) == E_Operators::equal)
        return true;
    return  false;
}

bool  Parser::isFunction(std::string& operand)
{
    if (operand.length() >= 3) {
        if (operand[operand.length() - 1] == E_Bracket::right 
                && operand[operand.length() - 2] == E_Bracket::left)
            return  true;
    }
    return false;
}

bool  Parser::isVariable(std::string& codePart)
{
    std::map<std::string, E_Type>::const_iterator iter = m_varNameType.find(codePart);
    if (iter == m_varNameType.end())
        return false;
    return true;
}


E_Reg Parser::findEmptyReg(E_Type type)
{
    int varType;
    switch (type) {
    case E_Type::BYTE:  
        varType = 1; 
        break;
    case E_Type::WORD:  
        varType = 2; 
        break;
    case E_Type::DWORD: 
        varType = 4; 
        break;
    case E_Type::QWORD: 
        varType = 8; 
        break;
    default:
        varType = 4;
        break;
    }
    std::map<E_Reg, bool>::const_iterator iter_begin = m_RegNameState.begin();
    std::map<E_Reg, bool>::const_iterator iter_end = m_RegNameState.end();

    int n_r = varType;
    for (; iter_begin != iter_end;) {
        if (iter_begin -> first % varType == 0 && iter_begin -> second == false) {
            return iter_begin -> first;
        }
        while (n_r != 0) {
            ++iter_begin;
            --n_r;
        }
        n_r = varType;
    }
    return E_Reg::undefined;
}

E_Ar  Parser::findEmptyAr()
{
    std::map<E_Ar, bool>::const_iterator iter_begin = m_addressRegistorState.begin();
    std::map<E_Ar, bool>::const_iterator iter_end = m_addressRegistorState.end();

    for (; iter_begin != iter_end; ++iter_begin) {
        if (iter_begin->second == false)
            return iter_begin->first;
    }
    return iter_begin->first;
}

E_Type Parser::getType(int& number)
{
    if (number > -128 && number < 127)
        return E_Type::BYTE;
    if (number > -8388608 && number < 8388607)
        return E_Type::WORD;
    if (number > -2147483647 && number < 2147483647)
        return E_Type::DWORD;
    if (number > -4294967296 && number < 429496729)
        return E_Type::QWORD;
    return E_Type::DWORD;
}

E_Type Parser::getRegType(int regNum)
{
    if (regNum % 8 == 0)
        return E_Type::QWORD;
    if (regNum % 2 == 0 && regNum % 4 != 0)
        return E_Type::WORD;
    if (regNum % 4)
        return E_Type::DWORD;
    return E_Type::BYTE;
}

const std::string  Parser::getTypename(E_Type type)
{
    switch (type) {
    case E_Type::BYTE:  
        return " BYTE ";
    case E_Type::WORD:  
        return " WORD ";
    case E_Type::DWORD: 
        return " DWORD ";
    case E_Type::QWORD: 
        return " QWORD ";
    default: 
        return " DWORD ";
    }
}

void  Parser::changeRegState(E_Reg reg)
{
    std::map<E_Reg, bool>::iterator iter = m_RegNameState.find(reg);
    iter->second = !iter->second;
}

void  Parser::changeArState(E_Ar reg)
{
    std::map<E_Ar,bool>::iterator iter = m_addressRegistorState.find(reg);
    iter->second = !iter->second;
}

void    Parser::generateCode(E_Reg left, E_Reg right, std::string& operator_1, int lineNumber, std::stringstream& t_codeStream, std::stringstream& t_dataStream, std::stack<E_Reg>& t_stackReg, std::stack<std::string>& t_stackOperator)
{
    E_Reg reg_3 = left > right ? left : right;
    E_Reg reg_4 = left < right ? left : right;
    if (operator_1 == "+")
        t_codeStream << "ADD " << getTypename(getRegType(reg_3)) << " R" << left << " , R" << right << " , R" << reg_3 << std::endl;
    else if (operator_1 == "-")
        t_codeStream << "SUB " << getTypename(getRegType(reg_3)) << " R" << left << " , R" << right << " , R" << reg_3 << std::endl;
    else if (operator_1 == "*")
        t_codeStream << "MUL " << getTypename(getRegType(reg_3)) << " R" << left << " , R" << right << " , R" << reg_3 << std::endl;
    else if (operator_1 == "/")
        t_codeStream << "DIV " << getTypename(getRegType(reg_3)) << " R" << left << " , R" << right << " , R" << reg_3 << std::endl;
    changeRegState(reg_4);
    t_stackOperator.pop();
    t_stackReg.push(reg_3);

    if (t_stackReg.size() == 2) {
        E_Reg reg_2 = t_stackReg.top();
        t_stackReg.pop();
        E_Reg reg_1 = t_stackReg.top();
        t_stackReg.pop();
        generateCode(reg_2, reg_1, t_stackOperator.top(), lineNumber, t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
    } else if (m_langSpecifiers.size() == 1 && t_stackReg.size() == 1 && t_stackOperator.size() == 1) {
        if (!isVariable(m_langSpecifiers.top()) && isFunction(m_langSpecifiers.top())) {
            E_Reg reg = t_stackReg.top();
            m_varNameType.insert(std::make_pair(m_langSpecifiers.top(), getRegType(reg)));
            changeRegState(reg);

            std::string temp_str;
            std::string temp_s = t_codeStream.str(); 
            t_codeStream.str(std::string());

            int i = 0;
            while (m_langSpecifiers.top()[i] != '(')
            {
                temp_str += m_langSpecifiers.top()[i];
                ++i;
            }
            t_codeStream << temp_str << ":" << std::endl;
            t_codeStream << "PUSH A1 " << std::endl;
            t_codeStream << "MOVE A0 , A1 " << std::endl;
            t_codeStream << temp_s ;
            t_codeStream << "MOVE R0 , R" << reg << std::endl;
            t_codeStream << "POP A1" << std::endl;
            t_codeStream << "RETURN" << std::endl;

            m_t_functionStream << t_codeStream.str() << std::endl;
            generateFunctionCall(m_langSpecifiers.top(), m_t_varStream);

            m_langSpecifiers.pop();
            changeRegState(reg);
            t_stackOperator.pop();
            t_stackReg.pop();
        } else  if (!isFunction(m_langSpecifiers.top()) && !isVariable(m_langSpecifiers.top())) {
            E_Reg reg = t_stackReg.top();
            getDataType(getRegType(reg), t_dataStream);
            m_varNameType.insert(std::make_pair(m_langSpecifiers.top(), getRegType(reg)));
            t_dataStream << m_langSpecifiers.top() << std::endl;
            t_codeStream << "ASSIGN  A" << findEmptyAr() << " , " << m_langSpecifiers.top() << std::endl;
            t_codeStream << "STORE  A" << findEmptyAr() << " , R" << reg << std::endl;
            m_t_varStream << t_codeStream.str() << std::endl;
            m_langSpecifiers.pop();
            changeArState(findEmptyAr());
            changeRegState(reg);
            t_stackOperator.pop();
            t_stackReg.pop();
        } else {
            throw ErrorMessage("\t[Parser ERROR]: Undefind variable or function in line  ", lineNumber);
        }
    }
}

void Parser::generateFunctionCall(std::string& temp, std::stringstream& t_codeStream)
{
    int i = 0;
    std::string temp_str;
    while (temp[i] != '(')
    {
        temp_str += temp[i];
        ++i;
    }
    t_codeStream << "ASSIGN  A" << findEmptyAr() << " , " << temp_str << std::endl;
    t_codeStream << "CALL  A" << findEmptyAr() << std::endl;
    changeArState(findEmptyAr());
    m_RegNameState.find(E_Reg::R0)->second = true;
}

void    Parser::parseLineCode(int lineNumber)
{
    std::stringstream        t_codeStream;
    std::stringstream        t_dataStream;
    std::stack<std::string>  t_stackOperator;
    std::stack<E_Reg>        t_stackReg;

    while (!m_langSpecifiers.empty()) {
        if (isRegistor(m_langSpecifiers.top())) {
            throw ErrorMessage("\t[Parser ERROR]: Invalid variable in line.", lineNumber); 
        }
        if (isOperator(m_langSpecifiers.top())) {
            t_stackOperator.push(m_langSpecifiers.top());
            m_langSpecifiers.pop();
        } else if (isNumber(m_langSpecifiers.top())) {
            if (isOperator(t_stackOperator.top())) {
                t_stackOperator.push(m_langSpecifiers.top());
                m_langSpecifiers.pop();
            } else  if (isNumber(t_stackOperator.top())) {
                std::string temp_1 = m_langSpecifiers.top();
                std::string temp_2 = t_stackOperator.top();
                m_langSpecifiers.pop();
                t_stackOperator.pop();
                int i = std::atoi(temp_1.c_str());
                E_Reg reg = findEmptyReg(getType(i));
                t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg << " , " << i << std::endl;
                changeRegState(reg);
                i = std::atoi(temp_2.c_str());
                E_Reg reg_2 = findEmptyReg(getType(i));
                t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg_2 << " , " << i << std::endl;
                changeRegState(reg_2);
                generateCode(reg, reg_2, t_stackOperator.top(), lineNumber,
                    t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
            } else  if (isFunction(t_stackOperator.top())) {
                std::string temp_1 = m_langSpecifiers.top();
                std::string temp_2 = t_stackOperator.top();
                m_langSpecifiers.pop();
                t_stackOperator.pop();
                int i = std::atoi(temp_1.c_str());
                E_Reg reg = findEmptyReg(getType(i));
                t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg << " , " << i << std::endl;
                changeRegState(reg);

                generateFunctionCall(temp_2, t_codeStream);

                generateCode(reg, E_Reg::R0, t_stackOperator.top(), lineNumber,
                    t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
                m_RegNameState.find(E_Reg::R0)->second = true;
            } else  if (isVariable(t_stackOperator.top())) {
                std::string temp_1 = m_langSpecifiers.top();
                std::string temp_2 = t_stackOperator.top();
                m_langSpecifiers.pop();
                t_stackOperator.pop();
                int i = std::atoi(temp_1.c_str());
                E_Reg reg = findEmptyReg(getType(i));
                t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg << " , " << i << std::endl;
                changeRegState(reg);
                std::map<std::string, E_Type>::iterator iter = m_varNameType.find(temp_2);
                E_Reg reg_2 = findEmptyReg(iter->second);
                changeRegState(reg_2);
                t_codeStream << "ASSIGN  A" << findEmptyAr() << " , " << temp_2 << std::endl;
                t_codeStream << "LOAD  R" << reg_2 << " , A" << findEmptyAr() << std::endl;
                changeArState(findEmptyAr());
                generateCode(reg, reg_2, t_stackOperator.top(), lineNumber, t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
            } else {
                throw ErrorMessage("\t[Parser ERROR]: Undefind variable or function in line ", lineNumber);
            }
        } else if (isFunction(m_langSpecifiers.top())) {
            if (isOperator(t_stackOperator.top())) {
                t_stackOperator.push(m_langSpecifiers.top());
                m_langSpecifiers.pop();
            } else  if (isNumber(t_stackOperator.top())) {
                std::map<std::string, E_Type>::iterator iter = m_varNameType.find(m_langSpecifiers.top());
                std::string temp_2 = m_langSpecifiers.top();
                m_langSpecifiers.pop();
                if (iter != m_varNameType.end()) {
                    generateFunctionCall(temp_2, t_codeStream);

                    int i = std::atoi(t_stackOperator.top().c_str());
                    E_Reg reg = findEmptyReg(getType(i));
                    t_stackOperator.pop();
                    t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg << " , " << i << std::endl;
                    changeRegState(reg);
                    generateCode(E_Reg::R0, reg, t_stackOperator.top(), lineNumber, t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
                    m_RegNameState.find(E_Reg::R0)->second = true;
                } else  if (m_langSpecifiers.size() == 0 && t_stackOperator.size() == 2) {
                    int i = 0;
                    std::string temp_str;
                    while (temp_2[i] != '(') {
                        temp_str += temp_2[i];
                        ++i;
                    }
                    i = std::atoi(t_stackOperator.top().c_str());
                    m_RegNameState.find(E_Reg::R0)->second = true;
                    E_Reg reg = findEmptyReg(getType(i));
                    m_varNameType.insert(std::make_pair(temp_2, getType(i)));

                    t_stackOperator.pop();
                    t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg << " , " << i << std::endl;

                    changeRegState(reg);
                    std::string temp_str_1 = t_codeStream.str();
                    t_codeStream.str(std::string());
                    t_codeStream << temp_str << ":" << std::endl;
                    t_codeStream << "PUSH A1 " << std::endl;
                    t_codeStream << "MOVE  A0 , A1" << std::endl;
                    t_codeStream << temp_str_1 ;
                    t_codeStream << "MOVE R0 , R" << reg << std::endl;
                    t_codeStream << "POP A1" << std::endl;
                    t_codeStream << "RETURN" << std::endl;

                    m_t_functionStream << t_codeStream.str() << std::endl;
                    generateFunctionCall(temp_2, m_t_varStream);
                }
            } else if (isFunction(t_stackOperator.top())) {
                std::map<std::string, E_Type>::iterator iter = m_varNameType.find(m_langSpecifiers.top());
                std::map<std::string, E_Type>::iterator iter_1 = m_varNameType.find(t_stackOperator.top());
                std::string temp_2 = m_langSpecifiers.top();
                generateFunctionCall(temp_2, t_codeStream);

                if (iter_1 == m_varNameType.end()) {
                    throw ErrorMessage("\t[Parser ERROR]: Undefind function in line ", lineNumber);
                }
                if (iter == iter_1) {
                    E_Reg reg = findEmptyReg(iter_1->second);
                    t_codeStream << "MOVE " << getTypename(iter->second) << " R0 , R" << reg << std::endl;
                }
                m_langSpecifiers.pop();
                if (iter == m_varNameType.end() && m_langSpecifiers.size() != 0)
                     throw ErrorMessage("\t[Parser ERROR]: Undefind function in line ", lineNumber);
                temp_2 = t_stackOperator.top();
                t_stackOperator.pop();
                generateFunctionCall(temp_2, t_codeStream);
                E_Reg reg = findEmptyReg(iter_1->second);
                changeRegState(reg);
                generateCode(E_Reg::R0, reg, t_stackOperator.top(), lineNumber, t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
                m_RegNameState.find(E_Reg::R0)->second = true;
            } else if (isVariable(t_stackOperator.top())) {
                std::map<std::string, E_Type>::iterator iter = m_varNameType.find(m_langSpecifiers.top());
                std::string temp_2 = m_langSpecifiers.top();
                if (iter == m_varNameType.end()) {
                     throw ErrorMessage("\t[Parser ERROR]: Undefind function in line ", lineNumber);
                }
                m_langSpecifiers.pop();
                generateFunctionCall(temp_2, t_codeStream);
                std::map<std::string, E_Type>::iterator iter_1 = m_varNameType.find(t_stackOperator.top());
                t_stackOperator.pop();
                E_Reg reg = findEmptyReg(iter_1->second);
                changeRegState(reg);
                generateCode(E_Reg::R0, reg , t_stackOperator.top(), lineNumber, t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
                m_RegNameState.find(E_Reg::R0)->second = true;
            } else {
                throw ErrorMessage("\t[Parser ERROR]: Undefind variable or function in line ", lineNumber);
            }
        } else  if (isVariable(m_langSpecifiers.top())) {
            if (isOperator(t_stackOperator.top())) {
                t_stackOperator.push(m_langSpecifiers.top());
                m_langSpecifiers.pop();
            } else  if (isNumber(t_stackOperator.top())) {
                std::map<std::string, E_Type>::iterator iter = m_varNameType.find(m_langSpecifiers.top());
                m_langSpecifiers.pop();
                int i = std::atoi(t_stackOperator.top().c_str());
                E_Reg reg_1 = findEmptyReg(getType(i));
                changeRegState(reg_1);
                E_Reg reg_2 = findEmptyReg(iter->second);
                t_stackOperator.pop();
                changeRegState(reg_2);
                t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg_1 << " , " << i << std::endl;
                t_codeStream << "ASSIGN  A" << findEmptyAr() << " , " << iter->first << std::endl;
                t_codeStream << "LOAD R" << reg_2 << " , A" << findEmptyAr() << std::endl;
                generateCode(reg_2, reg_1, t_stackOperator.top(), lineNumber, t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
            } else  if (isFunction(t_stackOperator.top())) {
                std::map<std::string, E_Type>::iterator iter_1 = m_varNameType.find(m_langSpecifiers.top());
                m_langSpecifiers.pop();
                std::map<std::string, E_Type>::iterator iter_2 = m_varNameType.find(t_stackOperator.top());
                std::string temp_2 = t_stackOperator.top();
                t_stackOperator.pop();
                if (iter_2 == m_varNameType.end()) {
                    throw ErrorMessage("\t[Parser ERROR]: Undefind function in line ", lineNumber);
                }
                generateFunctionCall(temp_2, t_codeStream);
                E_Reg reg = findEmptyReg(iter_1->second);
                changeRegState(reg);
                generateCode(reg, E_Reg::R0, t_stackOperator.top(), lineNumber, t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
            } else  if (isVariable(t_stackOperator.top())) {
                std::map<std::string, E_Type>::iterator iter_1 = m_varNameType.find(m_langSpecifiers.top());
                m_langSpecifiers.pop();
                std::map<std::string, E_Type>::iterator iter_2 = m_varNameType.find(t_stackOperator.top());
                t_stackOperator.pop();
                E_Reg reg = findEmptyReg(iter_1->second);
                changeRegState(reg);
                E_Reg reg_2 = findEmptyReg(iter_2->second);
                changeRegState(reg_2);
                generateCode(reg, reg_2, t_stackOperator.top(), lineNumber,
                    t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
            } else {
                throw ErrorMessage("\t[Parser ERROR]: Undefind function in line ", lineNumber);
            }
        } else  if (m_langSpecifiers.size() == 1 && t_stackOperator.size() == 2 && t_stackReg.empty()) {
            if (isNumber(t_stackOperator.top())) {
                int i = std::atoi(t_stackOperator.top().c_str());
                E_Reg reg = findEmptyReg(getType(i));
                m_varNameType.insert(std::make_pair(m_langSpecifiers.top(), getType(i)));
                getDataType(i, t_dataStream);
                t_dataStream << m_langSpecifiers.top() << std::endl;
                t_stackOperator.pop();
                t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg << " , " << i << std::endl;
                t_codeStream << "ASSIGN  A" << findEmptyAr() << " , " << m_langSpecifiers.top() << std::endl;
                t_codeStream << "STORE  A" << findEmptyAr() << " , R" << reg << std::endl;
                m_t_varStream << t_codeStream.str() << std::endl;
                m_langSpecifiers.pop();
                changeArState(findEmptyAr());
                changeRegState(reg);
            } else if (isFunction(t_stackOperator.top())) {
                std::map<std::string, E_Type>::iterator iter = m_varNameType.find(t_stackOperator.top());

                if (iter == m_varNameType.end()) {
                    throw ErrorMessage("\t[Parser ERROR]: Undefind function in line ", lineNumber);
                }
                E_Reg reg = findEmptyReg(iter->second);
                m_varNameType.insert(std::make_pair(m_langSpecifiers.top(), iter->second));
                getDataType(iter->second, t_codeStream);
                t_dataStream << m_langSpecifiers.top() << std::endl;
                generateFunctionCall(t_stackOperator.top(), t_codeStream);
                t_codeStream << "ASSIGN  A" << findEmptyAr() << " , " << m_langSpecifiers.top() << std::endl;
                t_codeStream << "STORE  A" << findEmptyAr() << " , R0" << reg << std::endl;
                m_langSpecifiers.pop();
                t_stackOperator.pop();
                changeArState(findEmptyAr());
                changeRegState(reg);
            } else {
                std::map<std::string, E_Type>::iterator iter = m_varNameType.find(t_stackOperator.top());
                if (iter == m_varNameType.end()) {
                    throw ErrorMessage("\t[Parser ERROR]: Undefind variable in line ", lineNumber);
                }
                E_Reg reg = findEmptyReg(iter->second);
                t_codeStream << "ASSIGN  A" << findEmptyAr() << " , " << t_stackOperator.top() << std::endl;
                t_codeStream << "LOAD  R" << reg << " , A" << findEmptyAr() << std::endl;
                changeArState(findEmptyAr());
                changeRegState(reg);
            }
        } else {
            while (t_stackOperator.size() != 0) {
                E_Reg reg = t_stackReg.top();
                std::string obj = t_stackOperator.top();
                changeRegState(reg);
                t_stackReg.pop();
                t_stackOperator.pop();
                std::string oper = t_stackOperator.top();
                int i = std::atoi(obj.c_str());
                E_Reg reg_2 = findEmptyReg(getType(i));
                changeRegState(reg_2);
                t_codeStream << "ASSIGN " << getTypename(getType(i)) << " R" << reg_2 << " , " << i << std::endl;
                generateCode(reg, reg_2, oper, lineNumber, t_codeStream, t_dataStream, t_stackReg, t_stackOperator);
            }
        }
    }
    m_dataSection << t_dataStream.str() ;
    updateRegAr();
    t_codeStream.clear();
    t_dataStream.clear();
}

void  Parser::getDataType(int argument, std::stringstream& t_dataStream)
{
    switch (getType(argument))
    {
    case E_Type::BYTE:
        t_dataStream << "BYTE  ";
        break;
    case E_Type::WORD:
        t_dataStream << "WORD  ";
        break;
    case E_Type::DWORD:
        t_dataStream << "DWORD  ";
        break;
    case E_Type::QWORD: 
        t_dataStream << "QWORD  ";
        break;
    default:
        t_dataStream << "DWORD  ";
        break;
    }
}

void  Parser::updateRegAr()
{
    for(auto&  element:m_addressRegistorState) {
         element.second = false;
    }
    m_addressRegistorState.find(E_Ar::SF)->second = true;
    m_addressRegistorState.find(E_Ar::SP)->second = true;

    for (auto& element : m_RegNameState) {
        element.second = false;
    }
    m_RegNameState.find(E_Reg::R0)->second = true;
}

void  Parser::generateCodeSectionCode()
{
    m_codeSection << "JUMP  START " << std::endl;
    m_codeSection << "EXIT" << std::endl;
    m_codeSection << m_t_functionStream.str() << std::endl;
    m_codeSection << "START:" << std::endl;
    m_codeSection << "PUSH A1 " << std::endl << "MOVE A0 , A1 " << std::endl;
    m_codeSection << m_t_varStream.str() << std::endl;
    m_codeSection << "POP A1" << std::endl << "RETURN" << std::endl;
}
