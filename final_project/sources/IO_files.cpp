#include "../includes/IO_files.hpp"
#include "../includes/error_message.hpp"

IO_Files::IO_Files(std::string& in, std::string& out)
{
    m_inputFile = in;
    m_outputFile = out;
}

void    IO_Files::readFromFile()
{
    std::string     readLine;
    std::fstream    inputFile;
    inputFile.open(m_inputFile, std::ios::in);

    if (!inputFile.is_open()) {
        throw ErrorMessage("\t[File ERROR]: Could not open input file!");
    } // if errorprone opening, throw error

    while (!inputFile.eof())
    {
        std::getline(inputFile, readLine);
        m_inFileStream << readLine;
    } // read from input file

    inputFile.close(); // close input file
}

std::stringstream  IO_Files::outWriteFileStream;

void    IO_Files::writeInFile()
{
    std::fstream    t_out;
    std::string     t_str = outWriteFileStream.str();
    
    t_out.open(m_outputFile, std::ios::out);

    if (!t_out.is_open()) {
        throw ErrorMessage("\t[File Error]: Could not open output file!");
    } // if errorprone opening, throw error
    
    t_out << t_str;
    t_out.close();

    std::cout << "\t[SUCCESS]: Assembly code now is in " << m_outputFile << " file!\n";
}

std::string IO_Files::getParsedCode()
{
    return m_inFileStream.str();
}

