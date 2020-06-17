/**
 * @file
 *
 * It is a pip struct and execute_pipeline function 
 */
#include <stdbool.h>
#ifndef _PIPI_H_
#define _PIPI_H_ 

/**
*it is struct of pipe to store the command line
*
*
*/
struct command_line {
    char **tokens;
    bool stdout_pipe;
    char *stdout_file;
};


int execute_pipeline(struct command_line *cmds);
#endif



