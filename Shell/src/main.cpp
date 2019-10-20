#include "builtin-manager.hpp"
#include "execution-manager.hpp"
#include "history-manager.hpp"
#include "string-processing.hpp"


int main()
{
    std::string input{}, last_cmd{};
    std::vector<char*> cmd1, cmd2;
    std::vector<std::string> tokens;
    bool is_concurrent;
    load_history();
    for(;;)
    {   
        //Get input
        for(;;)
        {
            std::cout << "\nosh>";
            std::getline(std::cin, input);

            //If user doesn't enter anything, prompt input again
            if (input.empty())
                continue;

            trim(input);
            //If input contains only whitespace (empty after trim), do as above
            if (input.empty())
                continue;

            is_concurrent = (input.back() == '&');
            if (is_concurrent)
                input.pop_back();

            if(process_history(last_cmd, input))
            {
                add_history(input);
                break;
            }

        }

        tokenize(tokens, input);

        last_cmd.swap(input);

        size_t i = 0;

        for (; i < tokens.size() 
        && tokens[i][0] != '<' && tokens[i][0] != '>' && tokens[i][0] != '|';
        ++i) 
        {
            cmd1.push_back(const_cast<char*>(tokens[i].data()));
        }
        cmd1.push_back(nullptr);


        //Case 1: No redirection or pipe
        if (i == tokens.size())
        {
            if (!exec_normal(cmd1, is_concurrent))
            {
                cmd1.clear();
                exit(1);
            }
        }

        //Case 2: Redirection
        else if (tokens[i] != "|")
        {
            if (!exec_redirect(cmd1, tokens[i].data(), tokens[i + 1].data(), is_concurrent))
            {
                cmd1.clear();                   
                exit(1);
            }
        }

        //Case 3: Pipe
        else
        {
            ++i;
            //Extract the 2nd command
            for (;i < tokens.size(); ++i)
            {
                cmd2.push_back(const_cast<char*>(tokens[i].data()));
            }
            cmd2.push_back(nullptr);
            
            if (!exec_pipe(cmd1, cmd2, is_concurrent))
            {
                cmd1.clear(); cmd2.clear();
                exit(1);
            }
            cmd2.clear();
        }

        cmd1.clear();
    }
    return 0;
}