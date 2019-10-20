#ifndef HISTORY_MANAGER_HPP
#define HISTORY_MANAGER_HPP

#include "string-processing.hpp"
#include <iostream>
#include <fstream>
std::vector<std::string> history_list;


bool process_history(std::string& last_input_cmd, std::string& input)
{
    auto i_str = input.substr(1);
    //If user is trying to re-exec an old command at specific index
    if (input[0] == '!' && is_positive_integer_string(i_str))
    {
        auto n = std::stoul(i_str);
        if (n > history_list.size())
        {
            std::cerr << "Index exceeds history size\n";
            return false;
        }
        input = history_list[n - 1];
        std::cout << "Executing: " << input << '\n';
        return true;
    }

    //If user is not asking for history
    if (input.compare(0, 2, "!!") != 0) 
        return true;

    else if (!last_input_cmd.empty())
    {
        input.swap(last_input_cmd);
        input += last_input_cmd.substr(2);
        std::cout << "Executing: " << input << '\n';
        return true;
    }
    std::cerr << "No recent history found";         
    return false;
}


void add_history(const std::string& input)
{
    if (history_list.empty() || history_list.back().compare(input) != 0)
        history_list.push_back(input);
    return;
}


void load_history()
{
    std::ifstream in("history.txt");
    if (in.is_open())
    {
        for(std::string buffer; std::getline(in, buffer);)
            history_list.emplace_back(buffer); 
        
        in.close();
    }
    else exit(1);
}


void save_history()
{
    std::string path("/home/");
    path += getenv("USER");
    path += "/Shell/history.txt";
    std::ofstream out(path.data(), std::ios::trunc);
    if (out.is_open())
    {
        for (auto const& cmd : history_list)
            out << cmd << '\n';
        out.close();
    }
    else exit(1);
}

#endif