#include "byte_compiler.h"
#include <algorithm>
using std::vector;
using std::binary_search;

//#define ASMI_DEBUG

ByteCompiler::ByteCompiler() {}

ByteCompiler::ByteCompiler(const std::string& fileNameToCompile)
{
	std::ifstream fileToCompile(fileNameToCompile);

	if (!fileToCompile.is_open())
	{
		std::cout << "Couldn't open file: " << fileNameToCompile << "\n";
		return;
	}

	while (fileToCompile.good())
	{
		std::string line;
		std::getline(fileToCompile, line);
		handleInput(line);
	}
	fileToCompile.close();
}

void ByteCompiler::handleInput(std::string& input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
	std::string word;
	for (const auto& x : input)
	{
		if (x == ',') {}
		else if (x == ' ')
		{ 
			code.push_back(word);
			word = "";
		}
		else
		{
			word += (unsigned char)x;
		}
	}
	code.push_back(word);
}

void ByteCompiler::compile(const std::string& fileNameOUT)
{
    code.pop_back();
	for (size_t i = 0; i < code.size(); i++)
	{
		auto result = std::find(commands.begin(), commands.end(), uppercase(code[i]));
		if (result == commands.end())
		{
			try
			{
				int value = std::stoi(code[i]);
				compiledBytes += (char)value;
				continue;
			}
			catch (std::exception& e)
			{
				std::cout << code[i] << " is not a valid command!\n";
				return;
			}
		}
		else
		{
			if (*result == "MOV")
			{
				if (uppercase(code[i + 2]) == "R1" || uppercase(code[i + 2]) == "R2" || uppercase(code[i + 2]) == "R3" || uppercase(code[i + 2]) == "R4" || uppercase(code[i + 2]) == "R5" || uppercase(code[i + 2]) == "R6" || uppercase(code[i + 2]) == "R7" || uppercase(code[i + 2]) == "R8")
				{
					compiledBytes += 3;
					continue;
				}
				else 
				{
					compiledBytes += 2;
					continue;
				}
			}
			compiledBytes += (result - commands.begin());
		}
	}
	std::ofstream file(fileNameOUT, std::fstream::trunc);
	for (const auto& x : compiledBytes)
	{
		file.put(x);
	}
	file.close();
	std::cout << "Compilation suceeded succesfully.\n";
}

void ByteCompiler::execute(AsmI& asmi, const std::string& fileNameIN)
{
	std::ifstream file(fileNameIN);
	if (!file.is_open())
	{
		std::cout << "Couldn't open file!\n";
	}
	std::string bytes;
	char c;
	while (file.get(c))
	{
#ifdef ASMI_DEBUG
		std::cout << (unsigned int) c << "\n";
#endif
		bytes += c;
	}

    vector<int> registers = {13, 14, 15, 16, 17, 18, 19, 20};
		
	for (size_t i = 0; i < bytes.size(); i++)
	{
		switch (bytes[i])
		{
		case 0:
			asmi.printFlags();
			break;
		case 1:
			asmi.printHelp();
			break;
		case 2:
			if (bytes[i+1] == R1_C)
			{
				asmi.setRegister("R1", bytes[i + 2]);
				i += 2;
				break;
			}
			else if (bytes[i + 1] == R2_C)
			{
				asmi.setRegister("R2", bytes[i + 2]);
				i += 2;
				break;
			}
			else if (bytes[i + 1] == R3_C)
			{
				asmi.setRegister("R3", bytes[i + 2]);
				i += 2;
				break;
			}

	        if (bytes[i+1] == R4_C)
			{
				asmi.setRegister("R4", bytes[i + 2]);
				i += 2;
				break;
			}
			else if (bytes[i + 1] == R5_C)
			{
				asmi.setRegister("R5", bytes[i + 2]);
				i += 2;
				break;
			}
			else if (bytes[i + 1] == R6_C)
			{
				asmi.setRegister("R6", bytes[i + 2]);
				i += 2;
				break;
			}
	        if (bytes[i+1] == R7_C)
			{
				asmi.setRegister("R7", bytes[i + 2]);
				i += 2;
				break;
			}
			else if (bytes[i + 1] == R8_C)
			{
				asmi.setRegister("R8", bytes[i + 2]);
				i += 2;
				break;
			}
			else
            { 
				std::cout << "Error! Invalid syntax!\n";
                break;
            }
		case 3:
			if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
			{
                if (binary_search(registers.begin(), registers.end(), bytes[i + 2]))
					asmi.mov(commands[bytes[i+1]-1], commands[bytes[i + 2]-1]);
				else
					std::cout << "Error! Invalid syntax!\n";
			}
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 2;
			break;
		case 4:
			if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
            {
			    if (binary_search(registers.begin(), registers.end(), bytes[i + 2]))
					asmi.orRegister(commands[bytes[i + 1]-1], commands[bytes[i + 2]-1]);
				else
					std::cout << "Error! Invalid syntax!\n";
			}
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 2;
			break;
		case 5:
            if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
            {
			    if (binary_search(registers.begin(), registers.end(), bytes[i + 2]))
					asmi.andRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				else
					std::cout << "Error! Invalid syntax!\n";
			}
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 2;
			break;
		case 6:
            if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
			{
                if (binary_search(registers.begin(), registers.end(), bytes[i + 2]))
					asmi.addRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				else
					std::cout << "Error! Invalid syntax!\n";
			}
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 2;
			break;
		case 7:
            if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
			{
                if (binary_search(registers.begin(), registers.end(), bytes[i + 2]))
					asmi.cmpRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				else
					std::cout << "Error! Invalid syntax!\n";
			}
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 2;
			break;
		case 8:
            if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
				asmi.pushRegister(commands[bytes[i + 1]]);
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 1;
			break;
		case 9:
            if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
				asmi.popRegister(commands[bytes[i + 1]]);
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 1;
			break;
		case 10:
            if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
			{
                if (binary_search(registers.begin(), registers.end(), bytes[i + 2]))
					asmi.subRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				else
					std::cout << "Error! Invalid syntax!\n";
			}
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 2;
			break;
		case 11:
            if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
            {
                if (binary_search(registers.begin(), registers.end(), bytes[i + 2]))
					asmi.divRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				else
					std::cout << "Error! Invalid syntax!\n";
			}
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 2;
			break;
		case 12:
            if (binary_search(registers.begin(), registers.end(), bytes[i + 1]))
			{
                if (binary_search(registers.begin(), registers.end(), bytes[i + 2]))
					asmi.mulRegister(commands[bytes[i + 1]], commands[bytes[i + 2]]);
				else
					std::cout << "Error! Invalid syntax!\n";
			}
			else
				std::cout << "Error! Invalid syntax!\n";
			i += 2;
			break;
		default:
			std::cout << "Error, invalid arguments!\n";
            break;
		}
	}
}

void ByteCompiler::printBytes()
{
	std::cout << "Length: " << compiledBytes.length() << '\n';
	
	for (auto& x : compiledBytes)
	{
		std::cout << (unsigned int)x << '\n';
	}
}


void ByteCompiler::printCode()
{
	for (auto x : code)
	{
		std::cout << x << '\n';
	}
}
