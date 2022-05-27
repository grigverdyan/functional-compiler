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

    while (!inputFile.eof()) {
        std::getline(inputFile, readLine);
        skipIdentifiers(readLine);
        std::string temp = skipComments(readLine);
        checkIn(temp);
        m_inFileStream << temp;
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
        throw ErrorMessage("\n\t[File Error]: Could not open output file!");
    } // if errorprone opening, throw error
    
    t_out << t_str;
    t_out.close();

    std::cout << "\n\t[SUCCESS]: Assembly code now is in " << m_outputFile << " file!\n";
}

std::string IO_Files::getParsedCode()
{
    return m_inFileStream.str();
}

void                IO_Files::skipIdentifiers(std::string& line)
{
    for (VectorString::iterator t = m_identifiers.begin(); t != m_identifiers.end(); ++t) {
        size_t pos = std::string::npos;

        // search for the substring in string in a loop untill nothing is found
        while ((pos  = line.find((*t)))!= std::string::npos) {
            // if found then erase it from string
            line.erase(pos, (*t).size());
        }
    }
}

std::string         IO_Files::skipComments(std::string& line)
{
    std::string newLine = line.substr(0, line.find("#"));
    return newLine;
}

void                IO_Files::checkIn(std::string& line)
{
    auto it = line.find("In()");
    if (it != std::string::npos) {
        std::cout << "\nWaiting for input: ";
        std::string input;
        std::cin >> input;
        line.replace(it, 4, input);
    }
}
