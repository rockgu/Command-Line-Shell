/**
 * @file
 *
 * Contains many builtin command execution method.
 */
#include<stdio.h>
#include<string.h>
#include "builtin.h"
#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include "logger.h"
#include "history.h"
#include "string_funcs.h"
#include "ui.h"

int change_dir (int argc, char *argv[]);
int exit_shell(int argc ,char *argv[]);
int history_execution (int argc,char *argv[]);
int print_history(int argc, char *argv[]);
void add_job(pid_t pid, char*command);
int tok_command(char **args,char *command);	
int fork_children(char **args,char *command);
//int history_execution (int argc);
int count =0 ; 
/**
*The struct to store the background job and pid
*
*/
struct job_list
{
	pid_t pid;
	char command[100];
};
struct job_list jobs[10];


/**
*add the job into array of job struct
*
*parameter: 
*
*pid : The pid number from the child
*
*command: The background command 
*
*return: 
*
*/
void add_job(pid_t pid, char *command)
{  
	

    if (count<10)
    {
		 jobs[count].pid = pid;
		 char *cmd = command;
		 int i =0 ;
		 while (*cmd != '\0')
		 {
		 	jobs[count].command[i] = *cmd;
		 	i++;
		 	cmd++;
		 }
		 jobs[count].command[i]='\0';

		 count++;
    }
   
    
	
}
/**
*print the background job
*
*parameter: 
*
* argc: the number of took word
*
* argv: array of the token word
*
*return: 0
*
*/


int print_job(int argc, char *argv[])
{
	for (int i =0 ; i<count;i++)
	{
		if (jobs[i].command[0]!='\0')
		{
		  printf("%s\n", jobs[i].command);	
		}
		
	}
	return 0;

}
/**
*remove the background job after they finish
*
*parameter: 
*
*pid: pid number of the children.
*
*return: 
*/

void remove_job(pid_t pid)
{
    
    //int value; 
    
	for(int i =0 ; i<10;i++)
	{
		if (jobs[i].pid ==pid)
		{
			//value = i;
			//free(jobs[i].command);
			jobs[i].pid = -1;
            jobs[i].command[0] = '\0';

		}
	}

	// for( int index =value; index<count-1;index++)
	// {
	// 	jobs[index] = jobs[index+1];
	// }
	// //	free(jobs[count].command);
	// jobs[count].pid = -1;
 //    jobs[count].command[0] = '\0';
 //    count --;
	
}



/**
*array of struct command
*
*/
struct builtin_def builtins[] ={
	{"cd",change_dir},
	{"exit",exit_shell},
	{"history",print_history},
	{"jobs",print_job}
};
/**
*Do the change the current direction
*
*parameter: 
*
* argc: the number of took word
*
* argv: array of the token word
*
* return: 0 or -1
*
*/
int change_dir(int argc,char *argv[])
{
	if(argc == 1){
		char home[100] = "/home/";
		char *user;
        user = getlogin();
        strcat(home,user);

		return chdir(home);
	}
	return chdir(argv[1]);

}
/**
*exit the shell
*
*parameter: 
*
* argc: the number of took word
*
* argv: array of the token word
*
* return: -2
*
*/
int exit_shell(int argc, char *argv[])
{	
    exit(0);
    return -2;
}
/**
*print the hostory of the command
*
*parameter: 
*
* argc: the number of took word
*
* argv: array of the token word
*
* return: 0 or -1
*
*/
int print_history(int argc, char *argv[])
{
  hist_print();
  return 0;
}
/**
*Do the change the current direction
*
*parameter: 
*
* argc: the number of took word
*
* argv: array of the token word
*
* return: 0 or -1
*
*/
int history_execution(int argc,char *argv[])
{  
	return 0;
}
/**
*handle all of command is not from exevp
*
*parameter: 
*
* argc: the number of took word
*
* argv: array of the token word
*
* return: 0 
*
*/
int handle_builtin (int argc,char *argv[])
{
	if (argv[0]==NULL){
		LOGP("Arg is NULL. Not a builtin.\n");
		return 1;
	}
	if (argv[0][0]=='!'){
		char str[100]={0};
		//strcpy(str,argv[0]);
		
		// int  i,len=strlen(argv[0]);
		// //LOG("$$$%d",len);
		int len =strlen(argv[0]);
		int j =0 ;
		for ( int i= 1; i<len;i++)
		{
			str[j]=argv[0][i];
			j++;
		}

		if (atoi(str) == 0)
		{
		
			if (str[0] =='!')
			{
				int num = hist_last_cnum();
				char *command = (char*)hist_search_cnum(num - 1);
				char **args = (char**)calloc(sizeof(char*) , 1000);
				tok_command(args,strdup(command));
                 hist_add(command);
			    fork_children(args,command);
	                
			}
			else
			{
				
				char* command = (char*)hist_search_prefix(str);
                if (command!= NULL)
                {
                	char **args = (char**)calloc(sizeof(char*) , 1000);
			        tok_command(args,strdup(command));
                    hist_add(command);
		            fork_children(args,command); 

                }

			}

		}
		else
		{
			int number = atoi(str);
			char *command = (char*)hist_search_cnum(number);
			if(command == NULL)
			{
				return 0; 
			}
		    char **args = (char**)malloc(sizeof(char*) * 1000);
			tok_command(args,strdup(command));
            hist_add(command);
		    fork_children(args,command);
			
		}
		return 0;
	}

	for( int i = 0; i<sizeof(builtins) /sizeof(struct builtin_def);++i){
		if(strcmp(argv[0],builtins[i].name)==0){
			return builtins[i].function(argc,argv);
		}

	}
	return -1;

}
/**
*token the command by the space
*
*parameter: 
*
* args: an array that store all the token word
*
* command: the command from shell
*
* return: 0 
*
*/
int tok_command(char **args,char *command)
{
	char *next_tok = command;
  //  LOG("sb command: %s\n", command);
    char *curr_tok;

    int count1 = 0;
   // LOG("sb command: %s\n", command);
    while ((curr_tok = next_token(&next_tok, " \t")) != NULL) {
        //realloc

       

        if (curr_tok[0]=='#')
        {
            break; 
        }
        

        args[count1++] = curr_tok;

       
    }
   // LOG("sb command: %s\n", command);

    args[count1] =NULL;
    return 0;
}
/**
*doing the execution to the commnad
*
*parameter: 
*
* args: an array that store all the token word
*
* command: the command from shell
*
* return: 0 or -1
*
*/

int fork_children(char **args,char *command)
{
	pid_t child;
        
    child = fork();
   // LOG("command   %s",user_command);

    if(child ==0){
        int ret = execvp(args[0],args);
        if (ret ==-1){

            perror("execvp");

        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        free(args);
        return EXIT_FAILURE;
    }else if(child == -1){
        perror("fork");
    }else{
        int status;
        
        waitpid(child,&status,0);
        set_emoji(status);

        
        
    }    
    return 0;       
    
}
