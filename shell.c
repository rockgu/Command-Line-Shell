/**
 * @file
 *
 * It is place to start crash
 */
#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


#include "history.h"
#include "logger.h"
#include "ui.h"
#include "string_funcs.h"
#include "builtin.h"
#include "pipe.h"


pid_t child;

/**
*signal handler for control c 
*
*parameter:
*
*/

void sigint_handler(int signo)
{
    printf("%c",'\n' );

}

/**
*signal handler for removing the finished background jobs' children
*
*parameter:
*
*/
void sigChild_handler()
{
    pid_t pid;
    int status;
    while ((pid =waitpid(-1,&status,WNOHANG))>0)
    {
       remove_job( pid);
    }
}


/**
 * 
 * start the crash shell !
 * 
 */

int main(void)
{

    init_ui();
    hist_init(100);      
    signal(SIGINT,sigint_handler);
    signal(SIGCHLD,sigChild_handler);
    
    char *command;
    struct command_line *cmds;
    
   
    
    while (true) {
        int background_check = 0;
        command = read_command();
        if (command == NULL) {
            break;
        }
      
        if (command[0]!='\0'&&command[0]!='!')
        {
            hist_add(command);
        }
        char *cmdCopy = strdup(command);
        

        char **args = (char**)malloc(sizeof(char*));
        char *next_tok = command;
       
        char *curr_tok;

        int count = 0; 
        int size1 = 1;
        int pipeCount =0;
        int redirectCount =0;
       
        while ((curr_tok = next_token(&next_tok, " \t")) != NULL) {

            // LOG("sb1 command: %s\n", command);
            // LOG("curr_tok  : %s\n", curr_tok);

            size1++;
            char** args1 = (char**)realloc(args, (size1 + 1)*sizeof(char*));
            if (args1 ==NULL)
            {
                free(args1);
            }
            else
            {
                args =args1;
            }
            if (curr_tok[0]=='#')
            {
                break; 
            }
            if(curr_tok[0]=='&')
            {
                background_check = 1;
                break;
            }
            if(curr_tok[0] == '|')
            {
               pipeCount++;
            }
            if(curr_tok[0] == '>')
            {
              redirectCount++;
            }  
            

            args[count++] = curr_tok;
            
        
            

           
        }
      //  LOG("sb command: %s\n", command);

        args[count] =NULL;

        if(pipeCount > 0 || redirectCount > 0)
            {
                cmds = malloc((pipeCount+redirectCount+1) * sizeof(struct command_line));
                int pipeNum = 0;
                int next = 0;
                cmds[pipeNum].tokens = malloc(sizeof(char *));
                cmds[pipeNum].stdout_pipe = true;
                for (int j = 0; j < count; j++) {
                    if (strcmp(args[j], "|") == 0) {
                        cmds[pipeNum].tokens[next] = NULL;
                        pipeNum++;
                        cmds[pipeNum].tokens = malloc(sizeof(char *));
                        cmds[pipeNum].stdout_pipe = true;
                        next = 0;
                    }
                    else if(strcmp(args[j], ">") == 0){
                        cmds[pipeNum].stdout_file = args[j+1];
                        break;
                    }
                    else {
                        cmds[pipeNum].tokens= realloc(cmds[pipeNum].tokens, sizeof(char *) * (next + 2));
                        cmds[pipeNum].tokens[next] = args[j];
                        next++;
                    }
                }
                cmds[pipeNum].tokens[next] = NULL;
                cmds[pipeNum].stdout_pipe = false;
            }
        

        if  (handle_builtin (count,args)==0){

        }
        else{

        
            child = fork();
          
            if (background_check ==1)
            {
               add_job(child, cmdCopy);
            }

            if(child ==0){

          //  LOG("check:  %s\n ",command);  
                if(pipeCount > 0 || redirectCount > 0)
                {
                     execute_pipeline(cmds);
                }
                else{

                int ret = execvp(command,args);
                if (ret ==-1){

                    perror("execvp");

                }
                close(STDIN_FILENO);
                close(STDOUT_FILENO);
                close(STDERR_FILENO);
                free(args);
                return EXIT_FAILURE;
               }
            }else if(child == -1){
                perror("fork");
            }else{
                int status;
                if(background_check == 0)
                {
                    waitpid(child,&status,0);
                    set_emoji(status);

                }
                
                
            }
            
            
        } 
        
        free(cmdCopy);
        free(args);  
    }
    hist_destroy();
    return 0;
}