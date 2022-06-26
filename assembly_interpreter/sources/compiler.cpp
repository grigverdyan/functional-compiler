#include <iostream>
#include <string>

#include "assembly_interpreter.h"
#include "byte_compiler.h"

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout << "Usage Help: ./compiler (1 for compilation, 2 for execution) input.asm out.exe \n";
		return 0;
	}
	if (argc >= 3)
	{
		std::string mode(argv[1]);
		std::string fileIN(argv[2]);
		if (mode == "1" && argc == 4) {
			std::string out(argv[3]);
			ByteCompiler compiler(fileIN);
			compiler.compile(out);
			return 0;
		}
		if (mode == "2" && argc == 3) {
			AsmI interpreter;
			ByteCompiler compiler;
			compiler.execute(interpreter, fileIN);
			interpreter.printRegisters();
			return 0;
		} else {
			std::cout << "Invalid arguments!\n";
			return 0;
		}
	}

    return 0;
}
