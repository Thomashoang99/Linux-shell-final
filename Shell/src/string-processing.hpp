#ifndef STRING_PROCESSING
#define STRING_PROCESSING

#include <sstream>
#include <vector>
#include <string>
#include <cstring>

void tokenize(std::vector<std::string>& tokens, const std::string& input_string)
{
    tokens.clear();
    std::istringstream buf(input_string);
    for (std::string token; buf >> token;)
    {
        tokens.push_back(token);
    }
    return;
}

void trim(std::string& str)
{
    auto i1 = str.find_first_not_of(' ');
    auto i2 = str.find_last_not_of(' ');

    if (i1 == std::string::npos)
        return str.clear();

    if (i1 == 0 && i2 == str.size() - 1)
        return;

    for(auto j = i1; j <= i2; ++j)
    {
        str[j - i1] = str[j];
    }
    str.erase(i2 - i1 + 1);
    return;
}


bool is_positive_integer_string(const std::string& input)
{
    size_t i = 0, s = input.size();
    for(; i < s; ++i)
    {
        if (input[i] < '0' || input[i] > '9')
            return false;
    }
    return true;
}

bool is_positive_integer_string(const char* input)
{
    size_t i = 0;
    for(; input[i] != '\0'; ++i)
    {
        if (input[i] < '0' || input[i] > '9')
            return false;
    }
    return true;

}

#endif