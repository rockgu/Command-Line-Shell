/**
 * @file
 *
 * It is user interface for user to use smoothly 
 */
#include <stdio.h>
#include <readline/readline.h>
#include <locale.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>


#include "history.h"
#include "logger.h"
#include "ui.h"

/* -- Private function forward declarations -- */
static int readline_init(void);
static int key_up(int count, int key);
static int key_down(int count, int key);
static char **command_completion(const char *text, int start, int end);
static char *command_generator(const char *text, int state);


static char prompt_str[80] ="╭─";
static char prompt_str2[80] = "╰─╼ ";
static bool scripting = false;
static char *line = NULL;
static size_t line_sz =0;
static int command_count =0;

int a = 0 ;

/**
*initial the UI and check it is scription or not
*
*parameter:
*
*/
void init_ui(void)
{
    LOGP("Initializing UI...\n");
  



    char *locale = setlocale(LC_ALL, "en_US.UTF-8");
    LOG("Setting locale: %s\n",
            (locale != NULL) ? locale : "could not set locale!");

    if (isatty(STDIN_FILENO)) {
        LOGP("stdin is a TTY; entering interactive mode\n");
        scripting = false;
    } else {
        LOGP("data piped in on stdin; entering script mode\n");
        scripting = true;
    }


    rl_startup_hook = readline_init;
}
/**
*determine the command is valid or not valid use the happy and cry face
*
*parameter:
*
*/

void set_emoji(int v )
{   
    if (v != 0)
    {
        a = 1; 
    }
    
    //printf("aa%d",a);
}

/**
*replace the user home directory to ~
*
*parameter: cwd : the current directory
*
*/
void current_dir(char *cwd)
{
  char *user;
  user = getlogin(); 
  char home[100] = "/home/";
  strcat(home,user);
  char *p = strstr(cwd,home);
  if (strncmp(cwd,home,strlen(home)) ==0) {
     p = p +strlen(home);
     char cwd1[100]="";
     strcat(cwd1,"~");
     strcat(cwd1,p);
     strcpy(cwd,cwd1);     
  }
  
}
/**
*print all the imformation for the pompt 
*
*parameter:
*
*/
char *prompt_line1(void) {
    char host [256];
    if ( gethostname(host,sizeof(host))== -1){
	 perror("gethostname() error");
    }   
    char *user;
    user = getlogin();
    char cwd[256];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL)
      perror("getcwd() error");
    static char prompt_str1[80] ="";
    strcpy(prompt_str1,prompt_str);
    strcat(prompt_str1,"-");
    strcat(prompt_str1,"[");
    if (a ==0 ){
        strcat(prompt_str1,"😀");
    }


    else{
        strcat(prompt_str1,"😰");
    }

    strcat(prompt_str1,"]");
    strcat(prompt_str1,"-");
    strcat(prompt_str1,"[");
    char str[20];
    sprintf(str, "%d", command_count);
    strcat(prompt_str1,str);

    strcat(prompt_str1,"]");
    strcat(prompt_str1,"-");
    strcat(prompt_str1,"[");
    strcat(prompt_str1,user);
    strcat(prompt_str1,"@");
    strcat(prompt_str1,host);
    current_dir(cwd);
    strcat(prompt_str1,cwd);
    
    
    strcat(prompt_str1,"]");  

    a = 0 ;
    return prompt_str1;
}

/**
*return the prompt str
*
*parameter:
*
*/
char *prompt_line2(void) {
    return prompt_str2;
}

/**
*read the command from the user input and check if it is scription or not 
*
*parameter:
*
*return: the command
*/
char *read_command(void)
{
    if(scripting == true)
    {
        // line = NULL;
        // line_sz = 0;
        ssize_t nread = getline(&line, &line_sz, stdin);
        if (nread == -1)
        {
            LOGP("reached end of input stream.\n");
            return NULL;
        }
        
        size_t newline = strcspn(line, "\n");
        line[newline] = '\0';
        return line;
    }
    else
    {
        puts(prompt_line1());
        command_count++;
        return readline(prompt_line2());
    }
}
/**
*init the readline
*
*parameter:
*
*return 0;
*/

int readline_init(void)
{
    rl_bind_keyseq("\\e[A", key_up);
    rl_bind_keyseq("\\e[B", key_down);
    rl_variable_bind("show-all-if-ambiguous", "on");
    rl_variable_bind("colored-completion-prefix", "on");
    rl_attempted_completion_function = command_completion;
    return 0;
}

/**
*press the key up
*
*parameter:
* 
*count
*
*key
*
*return 0
*
*/

int key_up(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'up' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: reverse history search

    return 0;
}
/**
*press the key down
*
*parameter:
*
*count 
*
*key 
*
*retrun 0; 
*/
int key_down(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'down' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: forward history search

    return 0;
}


/**
*finish the command  
*
*parameter:
*
*text:
*
*start:
*
*end:
*
*return 
*/
char **command_completion(const char *text, int start, int end)
{
    /* Tell readline that if we don't find a suitable completion, it should fall
     * back on its built-in filename completion. */
    rl_attempted_completion_over = 0;

    return rl_completion_matches(text, command_generator);
}

/**
 * This function is called repeatedly by the readline library to build a list of
 * possible completions. It returns one match per function call. Once there are
 * no more completions available, it returns NULL.
 */
char *command_generator(const char *text, int state)
{
    // TODO: find potential matching completions for 'text.' If you need to
    // initialize any data structures, state will be set to '0' the first time
    // this function is called. You will likely need to maintain static/global
    // variables to track where you are in the search so that you don't start
    // over from the beginning.

    return NULL;
}
