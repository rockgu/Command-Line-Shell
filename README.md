# Command Line Shell 

Author: Rock Gu 

## About This Project
In this project, we try to create our own command line shell. My command line shell is call crash.It can operate the basic system function in bin file. But it still has some extra builtin function such as cd exit and history. For the history function, you can type !num,!!, !prefix to find the history command. Our crash shell also have function on the multiple pipe and redirect. 

### What is Command Line Shell?
 Command Line Shell is a user interface for access to an operating system's services. Itt uses alphanumeric characters typed on a keyboard to provide instructions and data to the operating system, interactively.


### Included Files
There are several files included. These are:
   - <b>Makefile</b>: Including to compile and run the program.
   - <b>shell.c</b>: The program driver
   - <b>history.c</b>: Include the data struction to contain the history of command and all the history execution fuction
   - <b>builtin.c</b>: It will have all the system fuction which is not in the bin file and also contain the data structure of background job and execution function
   - <b>ui.c</b>: It contains the the all the user interface. 
   - <b>String_funcs.c</b>: It contains the helper function which will token all of command. 
   - <b>pip.c</b>: It contains pip data structure and executing function. 
   - <b>logger.c</b>: Helper class for debug


There are also header files for each of these files.


To compile and run:

```bash
make
./crash
```


### Program Output
```bash
[hgu4@rock P2-rockgu]$ ./crash 
ui.c:47:init_ui(): Initializing UI...
ui.c:53:init_ui(): Setting locale: en_US.UTF-8
ui.c:57:init_ui(): stdin is a TTY; entering interactive mode
╭─-[😀]-[0]-[hgu4@rock~/P2-rockgu]
╰─╼ ls
history.c:72:hist_add(): inserting ls at 0
a.out	   builtin.h	  builtin.o  docs	history.h      history.o    logger.h	  Makefile  pipe.h	pipe.o	   shell.c  string_funcs.c  string_funcs.h.gch	tests  ui.h	 ui.o
builtin.c  builtin.h.gch  crash      history.c	history.h.gch  libshell.so  logger.h.gch  pipe.c    pipe.h.gch	README.md  shell.o  string_funcs.h  string_funcs.o	ui.c   ui.h.gch
╭─-[😀]-[1]-[hgu4@rock~/P2-rockgu]
╰─╼ pwd
history.c:72:hist_add(): inserting pwd at 1
/home/hgu4/P2-rockgu
╭─-[😀]-[2]-[hgu4@rock~/P2-rockgu]
╰─╼ history
history.c:72:hist_add(): inserting history at 2
1    ls
2    pwd
3    history
╭─-[😀]-[3]-[hgu4@rock~/P2-rockgu]
╰─╼ !p
history.c:72:hist_add(): inserting pwd at 3
/home/hgu4/P2-rockgu
╭─-[😀]-[4]-[hgu4@rock~/P2-rockgu]
╰─╼ 



```

## Testing

To execute the test cases, use `make test`. To pull in updated test cases, run `make testupdate`. You can also run a specific test case instead of all of them:

```
# Run all test cases:
make test

# Run a specific test case:
make test run=4

# Run a few specific test cases (4, 8, and 12 in this case):
make test run='4 8 12'
```
