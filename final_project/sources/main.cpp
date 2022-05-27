#include "../includes/error_message.hpp"
#include "../includes/IO_files.hpp"
#include "../includes/lexer.hpp"
#include "../includes/header.hpp"
#include "../includes/parser.hpp"

int main(int argc, char *argv[])
{
    try {
       if (argc < 3) {
           throw ErrorMessage("\t[File ERROR]: No input/output file included!");
        }
    } catch (ErrorMessage& error) {
        error.showErrorMessage();
        exit(1);
    }

    std::string input = argv[1];
    std::string output = argv[2];
    try {
        if (!isValidFiles(input, output)) {
            throw ErrorMessage("\t[File ERROR]: Not valid file extension. Must be .txt");
        } 
    } catch (ErrorMessage& error) {
        error.showErrorMessage();
        exit(2);
    }

    input = "data/" + input;
    output = "data/" + output;
    try {
        IO_Files    IO_obj(input, output);  // initialize input/output file object
        IO_obj.readFromFile();              // read from file
        
        std::string obj = IO_obj.getParsedCode(); // get parsed code
        Lexer       lexer_obj(obj);               // initialize lexer object
        lexer_obj.codeLineLexemme();
        int lineNumber = 1;                // code line number
        Parser  parser_obj;
//        lexer_obj.m_parsedCode.push("");
        while (!lexer_obj.m_parsedCode.empty()) {
            parser_obj.lexemmeToTokenString(lexer_obj.m_parsedCode.front(), lineNumber);
            lexer_obj.m_parsedCode.pop();
            parser_obj.parseLineCode(lineNumber);
            lineNumber++;
        }
        parser_obj.generateOutputCode();
        IO_obj.writeInFile();
    } catch (ErrorMessage& error) {
        error.showErrorMessage();
        exit(3);
    }

    return 0;
}
