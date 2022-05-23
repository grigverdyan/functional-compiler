#ifndef IO_FILES_H
#define IO_FILES_H

#include "../includes/header.hpp"

class   IO_Files {
public:
    IO_Files(std::string& in, std::string& out);

    static std::stringstream    outWriteFileStream;
    void                readFromFile();
    void                writeInFile();
    std::string         getParsedCode();
private:
    std::string         m_inputFile;
    std::string         m_outputFile;
    std::stringstream   m_inFileStream;
};

#endif // IO_FILES_H
