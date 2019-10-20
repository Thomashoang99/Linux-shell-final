#ifndef BUILTIN_CMD_HELPER
#define BUILTIN_CMD_HELPER

#include "history-manager.hpp"
#include <unistd.h>

inline int change_dir(const std::vector<char*>& args)
{
    if (args.size() == 2 && args[1] == nullptr)
        return 0;
    std::string path;
    for (size_t i = 1, s = args.size() - 1; i < s; ++i)
    {
        path += args[i]; path.push_back(' ');
    }
    path.pop_back();
    return chdir(path.data());
}


inline int history(const std::vector<char*>& args)
{
    if (args.size() > 2)
    {
        if (strcmp(args[1], "-c") == 0)
            history_list.clear();
        else if (is_positive_integer_string(args[1]))
        {
            auto count = std::stoul(std::string(args[1]));
            for (
            auto s = history_list.size(), i = (count > s ? 0 : s - count); 
            i < s; ++i
            )
            {
                std::cout << i + 1 << " " << history_list[i] << '\n';
            }
        }
    }
    else
    {
        size_t i = 0;
        for (auto const &saved_cmd : history_list)
            std::cout << ++i << " " << saved_cmd << '\n';
    }
    return 0;
}

inline int pwd(const std::vector<char*>& args)
{
    std::cout << get_current_dir_name() << '\n';
    return 0;
}


inline bool is_builtin_cmd(const std::vector<char*>& args)
{
    return 
    strcmp(args[0], "exit") == 0 ||
    strcmp(args[0], "cd") == 0 ||
    strcmp(args[0], "pwd") == 0 ||
    strcmp(args[0], "history") == 0;
}

inline int exec_builtin_cmd(const std::vector<char*>& args)
{
    if (strcmp(args[0], "exit") == 0)
    {
        save_history();
        exit(0); return 0;
    }
    if (strcmp(args[0], "cd") == 0)
    {
        return change_dir(args);
    }
    if (strcmp(args[0], "history") == 0)
    {
        return history(args);
    }
    if (strcmp(args[0], "pwd") == 0)
    {
        return pwd(args);
    }
    return 1;
}

#endif