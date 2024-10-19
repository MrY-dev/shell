# Structure 
## main.h
All the headers used are declared in this file along with  function declarations which are used
across the files.
## main.cpp
this is where the main function resides where the loop runs continously displaying the prompt,
small functions like search,pwd,echo are implemented here.
## ls.cpp
implementation of ls
## parsexec.cpp
this is where the parsing of command and execution order is implemented including pipe,IO redirection,etc.
## redirect.cpp
basic redirection without pipe is implemented
## signals
SIGINT,SIGTSTP signal handlers are installed , Ctrl-D will kill terminate shell.

## Makefile and Build
Makefile is made with the target name as main, just run make file and execute ./main for the shell to run.
