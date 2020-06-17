/**
 * @file
 *
 * Contains history and job list data structure and method
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "history.h"
#include "logger.h"
#include <stdbool.h>


/**
*
*It is a struct of history command and number. 
*
*/
struct command_history {
  char command[100];
  unsigned int history;
  };



static int count =1;
int limit1; 

struct command_history *commands_his;


/**
*declare the size of the command_history struct 
*
*parameter:
*
*limit : the size of the command_history struct 
*
*
*/
void hist_init(unsigned int limit)
{
  limit1 = limit;
  commands_his = calloc(limit,sizeof(struct command_history));
  
}

/**
*destory command_history struct 
*
*parameter:
*
*
*/
void hist_destroy(void)
{
   free(commands_his);
}

/**
*adding the element to the command_history struct
*
*parameter:
*
*
*/
void hist_add(const char *cmd)
{
   
  int index = (count - 1 )%100;
  LOG("inserting %s at %d\n", cmd, index);
  int i = 0;
  while(*cmd !=0){
    commands_his[index].command[i] =*cmd;
    i++;
    cmd++;
  }
  commands_his[index].command[i]='\0';
  commands_his[index].history=count++;
  
}

/**
*print the element from command_history struct 
*
*parameter:
*
*/
void hist_print(void)
{
  if (count < limit1){
    for ( int i=0 ; i<count - 1;i++){
      printf("%u    %s\n",commands_his[i].history, commands_his[i].command);
    }
  }
  else{
    int index = (count - 1) %limit1;
    
    for (int i = index ;i<limit1;i++ )
    {
      printf("%u    ",commands_his[i].history);
      printf("%s\n", commands_his[i].command);

    }
    for (int i = 0 ;i<index  ;i++ )
    {
      printf("%u    ",commands_his[i].history);
      printf("%s\n", commands_his[i].command);

    }
  }
  fflush(stdout);
    
}

/**
*search the match prefix command from the command_history struct 
*
*parameter: prefix of potential searching result
*
*return: the searching result form command_history struct
*
*/

const char *hist_search_prefix(char *prefix)
{
  bool check;
  if (count < limit1){
    for ( int i=count - 2 ; i>=0;i--)
    {
      check = true; 
      int size1 = strlen(commands_his[i].command);
      int size2 = strlen(prefix);
      if (size1>=size2)
      {
        for (int j =0 ; j<size2 ;j++)
          {
            if(*prefix !=commands_his[i].command[j])
            {
              check = false;
              break;
            }               
               prefix++;         

          } 
          if(check == true)
          {
            return commands_his[i].command;
          } 

          
      }
    }
  }
  else{
    int index = (count - 1) %limit1;
    
    for (int i = index-1 ;i>=0  ;i-- )
    {
      check = true; 
      int size1 = strlen(commands_his[i].command);
      int size2 = strlen(prefix);
      if (size1>=size2)
      {
        for (int j =0 ; j<size2 ;j++)
          {
            if(*prefix !=commands_his[i].command[j])
            {
              check = false;
              break;
            }               
               prefix++;         

          } 
          if(check == true)
          {
            return commands_his[i].command;
          } 

          
      }

    }
    for (int i = limit1-1 ;i>=index;i-- )
    {
      check = true; 
      int size1 = strlen(commands_his[i].command);
      int size2 = strlen(prefix);
      if (size1>=size2)
      {
        for (int j =0 ; j<size2 ;j++)
          {
            if(*prefix !=commands_his[i].command[j])
            {
              check = false;
              break;
            }               
               prefix++;         

          } 
          if(check == true)
          {
            return commands_his[i].command;
          } 

          
      }
    }
    
  }

    return NULL;
}

/**
*find the right command that match the history number
*
*parameter: 
*
*command_number: The number of the history that you need to search
*
*limit : the size of the command_history struct 
*
*
*/
 
const char *hist_search_cnum(int command_number)
{
    
    if (command_number <= 0) {
        return NULL;
    }
    int index = (command_number - 1) % limit1;
    if (commands_his[index].history != command_number) {
        return NULL;
    }
    return commands_his[index].command;
}

/**
*find the latest history number 
*
*parameter:
*
*
*return: The most recently history number. 
*
*
*/
unsigned int hist_last_cnum(void)
{
    // TODO: Retrieve the most recent command number.
    
    return count;
}
