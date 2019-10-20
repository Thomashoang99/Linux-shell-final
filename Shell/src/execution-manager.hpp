#ifndef EXECUTION_MANAGER_HPP
#define EXECUTION_MANAGER_HPP

#include "builtin-manager.hpp"
#include <sys/wait.h>
#include <fcntl.h>

bool exec_pipe(
    const std::vector<char*>& first_cmd, const std::vector<char*>& second_cmd, 
    bool is_concurrent)
{
    int fd[2];
    if (pipe(fd) < 0)
        return false;

    auto p1 = fork();
    if (p1 < 0)
        return false;

    //Child 1 execution at write end
    if (p1 == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        if (execvp(first_cmd[0], first_cmd.data()) < 0)
        {
            perror(first_cmd[0]);
            return false;
        }
    }
    //Parent execution, forking 2nd child
    else
    {
        auto p2 = fork();
        if (p2 < 0)
            return false;

        //Child 2 execution at read end
        if (p2 == 0)
        {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            if (execvp(second_cmd[0], second_cmd.data()) < 0)
            {
                perror(second_cmd[0]);
                return false;
            }
        }

        else
        {
            if (!is_concurrent)
            {
                int status;
                waitpid(p1, &status, WUNTRACED); 
            }
        }
    }
    return true;
}

bool exec_redirect(
    const std::vector<char*>& cmd, 
    const char* opr, const char* file, 
    bool is_concurrent)
{
    int fd;
    
    if (is_builtin_cmd(cmd))
    {
        if (opr[0] == '>') 
        {
            if (opr[1] == '>')
                fd = open(file, O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
            else
                fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
            
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (opr[0] == '<')
        {
            fd = open(file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        return !exec_builtin_cmd(cmd);
    }

    auto pid = fork();
    if (pid < 0)
        return false;

    if (pid == 0)
    {
        if (opr[0] == '>') 
        {
            if (opr[1] == '>')
                fd = open(file, O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
            else
                fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
            
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (opr[0] == '<')
        {
            fd = open(file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (execvp(cmd[0], cmd.data()) < 0)
        {
            perror(cmd[0]);
            return false;
        }
    }
    
    else
        if (!is_concurrent)
        {
            int status;
            waitpid(pid, &status, WUNTRACED);
        }
    return true;
}


bool exec_normal(const std::vector<char*>& cmd, bool is_concurrent)
{
    if (is_builtin_cmd(cmd))
    {
        return !exec_builtin_cmd(cmd);
    }
    auto pid = fork();
    if (pid < 0)
    {
        perror(cmd[0]); 
        return false;
    }
    if (pid == 0)
    {
        if (execvp(cmd[0], cmd.data()) < 0)
        {
            perror(cmd[0]);
            return false;
        }
    }
    else
        if (!is_concurrent)
        {
            int status;
            waitpid(pid, &status, WUNTRACED);
        }
    return true;
}

#endif