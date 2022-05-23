#include "../includes/error_message.hpp"

ErrorMessage::ErrorMessage(std::string message)
{
    m_errorMessage = message;
}

ErrorMessage::ErrorMessage(std::string message, size_t line)
{
    m_errorMessage = message;
    m_errorLine = line;
}

std::string ErrorMessage::getErrorMessage()
{
    return m_errorMessage;
}

size_t  ErrorMessage::getErrorLine()
{
    return m_errorLine;
}

void    ErrorMessage::showErrorMessage()
{
    std::cout << std::endl;
    if (m_errorLine != 0) {
        std::cerr << m_errorMessage << std::to_string(m_errorLine);
    } else {
        std::cerr << m_errorMessage;
    }

    std::cout << std::endl;
}
