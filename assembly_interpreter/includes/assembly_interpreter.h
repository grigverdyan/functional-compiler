#ifndef ASSEMBLY_INTERPRETER
#define ASSEMBLY_INTERPRETER

#include <cstdint>
#include <iostream>
#include <string>
#include <bitset>
#include <stack>
#include <vector>

typedef uint8_t u8;
typedef unsigned int ui;
enum { R1, R2, R3, R4, R5, R6, R7, R8, NUM_REGISTERS };

class AsmI
{
public:
	AsmI();
	void handleInput(const std::string& input);
	void command();
	int getRegisterID(const std::string& name);
	void setRegister(const std::string& registerName, u8 value);
	void printRegisters();
	void printBits(const std::string& registerName);
	void mov(const std::string& arg1, const std::string& arg2);
	void orRegister(const std::string& arg1, const std::string& arg2);
	void andRegister(const std::string& arg1, const std::string& arg2);
	void addRegister(const std::string& arg1, const std::string& arg2);
	void subRegister(const std::string& arg1, const std::string& arg2);
	void mulRegister(const std::string& arg1, const std::string& arg2);
	void divRegister(const std::string& arg1, const std::string& arg2);
	void cmpRegister(const std::string& arg1, const std::string& arg2);
	void pushRegister(const std::string& arg);
	void popRegister(const std::string& registerName);
	void printFlags();
	void printHelp();
private:
	std::vector<std::string> arguments;
	bool isEqual;
	struct Register
	{
		u8 registerValue;
		int registerID;
	};
	Register registers[NUM_REGISTERS];
	std::string registerNames[NUM_REGISTERS] = { "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8" };
	std::stack<u8> stack;
};

#endif  /* ASSEMBLY_INTERPRETER */
