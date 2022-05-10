#include "header.hpp"

std::fstream output("output.txt", std::ios::out);

void    postfixToAssembly(string& postfix, vector<string>& table, vector<string>& expr_vec, string& function_name)
{
    std::stack <string> st;
    string operand2;
    string operand1;
    string temp_reg;
    string ch;
    
    if (!output.is_open())
    {
        std::cerr << "ERROR: can not open file \"output.txt\".";
        exit(1);
    }

    output << endl << function_name  << endl;
    
    string reg = "R1";
    for (string& ch : expr_vec)
    {
        if (ch[0] != '+' && ch[0] != '-' && ch[0] != '*' && ch[0] != '/' && ch[0]!= '=')
            st.push(ch);
        else
        {
            operand2 = st.top();
            st.pop();
            
            operand1 = st.top();
            st.pop();
           
            if (!(operand1[0] == 'R' && (operand1[1] >= '1'
                && operand1[1] <= '9') && operand2[0] == 'R'
                && (operand2[1] >= '1' && operand2[1] <= '9')))
            {
                if (isInTable(operand1, table))
                    output << "MVI " << reg << "," << operand1 << endl << "LOAD " << reg << "," << reg << endl;
                else if (operand1[0] == 'R' && (operand1[1] >= '1' && operand1[1] <= '9'))
                    reg[1] -= 1;
                else
                    output << "MVI " << reg << "," << operand1 << endl;
                st.push(reg);
                reg[1] += 1;
                temp_reg = reg;
                if (isInTable(operand2, table))
                    output << "MVI " << reg << "," << operand2 << endl << "LOAD " << reg << "," << reg << endl;
                else if (operand2[0] == 'R' && (operand2[1] >= '1' && operand2[1] <= '9'))
                    reg = operand2;
                else
                    output << "MVI " << reg << "," << operand2 << endl;
                st.push(reg);
                reg = temp_reg;
                reg[1] -= 1;
            }
            
            switch(ch[0])
            {
                case '+': 
                    output << "ADD "; 
                    break;
                case '-': 
                    output << "SUB "; 
                    break;
                case '*': 
                    output << "MUL "; 
                    break;
                case '/': 
                    output << "DIV "; 
                    break;
                case '=': 
                    output << "STORE "; 
                    break;
            }
           if (!(operand1[0] == 'R' && (operand1[1] >= '1'
                && operand1[1] <= '9') && operand2[0] == 'R'
                && (operand2[1] >= '1' && operand2[1] <= '9')))
           {
                operand2 = st.top();
                st.pop();
                operand1 = st.top();
                st.pop();
           }

            if (ch[0] == '=') 
                output << operand1;
            else if (st.empty() == false)
                output << reg << " ";
            else
            {
                reg = "R1";
                output << reg << " ";
            }
            
            if (ch[0] == '=')
                output << operand2;
            else
                output << operand1 << "," << operand2 << endl;
            st.push(reg);
            reg[1] += 1;
        }
    }
    output << endl << endl;
}

int isInTable(string& str, vector<string>& table)
{
    size_t size = table.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (str == table[i])
            return 1;
    }
    return 0;
}
