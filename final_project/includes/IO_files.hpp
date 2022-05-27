#ifndef IO_FILES_H
#define IO_FILES_H

#include "../includes/header.hpp"

//aliases
using VectorString = std::vector<std::string>;

class   IO_Files {
public:
    IO_Files(std::string& in, std::string& out);
    VectorString    m_identifiers = {
        "VAR", "FUNC", "Begin", "End"   
    };

    static std::stringstream    outWriteFileStream;
    void                readFromFile();
    void                writeInFile();
    void                checkIn(std::string& line);
    void                skipIdentifiers(std::string& line);
    std::string         skipComments(std::string& line);
    std::string         getParsedCode();

private:
    std::string         m_inputFile;
    std::string         m_outputFile;
    std::stringstream   m_inFileStream;
};

#endif // IO_FILES_H
