/**
 * @file
 *
 * Contains many builtin command execution method.
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#ifndef _BUTLTIN_H_
#define _BUTLTIN_H_

/**
*
*The struct of builtin command 
*
*/
struct builtin_def
{
	char name[128];
	int(*function)(int argc, char*argv[]);
	
};

void add_job(pid_t pid, char*command);
int print_job(int argc, char *argv[]);
void remove_job(pid_t pid);


int handle_builtin(int argc,char *argv[]);
#endif