#ifndef BYTE_COMPILER
#define BYTE_COMPILER

#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "assembly_interpreter.h"
#include "helpers.h"

enum {
	FLAGS, HELP, MOV_V, MOV_R, OR, AND,
	ADD, CMP, PUSH, POP, SUB, DIV, MUL,
	R1_C, R2_C, R3_C, R4_C, R5_C, R6_C,
    R7_C, R8_C
};

class ByteCompiler
{
	uint8_t value_stack[1024];
	uint8_t instruction_stack[1024];
	std::string compiledBytes;
	std::vector<std::string> code;
	std::vector<std::string> commands = { 
        "FLAGS",  "HELP", "MOV", "MOV_V", "OR", "AND", "ADD", "CMP", "PUSH", "POP", "SUB", 
        "DIV", "MUL", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "NULL" };
public:
    ByteCompiler();
	ByteCompiler(const std::string& fileNameToCompile);
	void handleInput(std::string& input);
	void compile(const std::string& fileNameOUT);
	void execute(AsmI& asmi, const std::string& fileNameIN);
	void printBytes();
	void printCode();
};

#endif  /* BYTE_COMPILER */
