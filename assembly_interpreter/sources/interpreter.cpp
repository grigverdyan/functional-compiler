#include "assembly_interpreter.h"
#include <iostream>
#include <string>

int main()
{
	AsmI asmi;
	asmi.printHelp();

	while (1) {
		std::string line;
		std::getline(std::cin, line);
		asmi.handleInput(line);
		asmi.command();
		asmi.printRegisters();
	}

    return 0;
}
