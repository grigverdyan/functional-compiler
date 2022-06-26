#include "helpers.h"

std::string lowercase(const std::string& string)
{
	if (std::isalpha(string.at(0))) {
		std::string result;
		for (auto& x : string)
        {
			result += std::tolower(x);
		}
		return result;
	}
	else return string;
}

std::string uppercase(const std::string& string)
{
	try {
		if (std::isalpha(string.at(0))) {
			std::string result;
			for (auto& x : string)
			{
				result += std::toupper(x);
			}
			return result;
		} else {
            return string;
        }
	} catch (std::exception& e) {
		return string; 
	}
}
