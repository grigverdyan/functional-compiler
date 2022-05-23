#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

#include "header.hpp"

class   ErrorMessage {
public:
    ErrorMessage(std::string message);
    ErrorMessage(std::string message, size_t line);

    void        showErrorMessage();
    std::string getErrorMessage();
    size_t      getErrorLine();

private:
    std::string m_errorMessage;
    size_t      m_errorLine;

};

#endif // ERROR_MESSAGE_H
