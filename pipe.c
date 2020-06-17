/**
 * @file
 *
 * Contains pipe data structures and executing functions.
 */
#include "pipe.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * Execute pipeline.
 *
 * @param cmds Command to execute.
 * @return 0 if successful.
 */
int execute_pipeline(struct command_line *cmds)
{
    while (cmds->stdout_pipe == true)
    {
        int fd[2];
        if (pipe(fd) == -1) {
            perror("pipe");
            return -1;
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            int ret = execvp(cmds->tokens[0], cmds->tokens);
            if (ret == -1)
            {
                perror("execvp");
                return -1;
            }
        }
        else if(pid == -1){
            perror("fork");
        }
        else
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);
        }
        cmds++;
    }

    if (cmds->stdout_file != NULL)
    {
        int fd_out = open(cmds->stdout_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
        if (fd_out == -1)
        {
            perror("open");
            return -1;
        }
        dup2(fd_out, STDOUT_FILENO);
        close (fd_out);
        int ret = execvp(cmds->tokens[0], cmds->tokens);
        if (ret == -1)
        {
            perror("execvp");
            return -1;
        }
    }
    else
    {
        int ret = execvp(cmds->tokens[0], cmds->tokens);
        if (ret == -1)
        {
            perror("execvp");
            return -1;
        }
    }
    return 0;
}