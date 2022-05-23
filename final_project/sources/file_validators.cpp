#include "../includes/header.hpp"

bool    isValidFiles(std::string in, std::string out)
{
    size_t  inExtIdx = in.find(".txt");
    size_t  outExtIdx = out.find(".txt");
    return (inExtIdx == std::string::npos || outExtIdx == std::string::npos
            || in.length() != (inExtIdx + 4) || out.length() != (outExtIdx + 4)
            || in[0] == '.' || out[0] == '.') ? false : true;
}
