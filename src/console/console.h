//  Brian Guidarini
//  October 10th, 2019
//  Header file for the console file. It just contains prototypes.
int executeCommand(char**, int&, int, char**);
void printEnviron(char**, int);
void stopItGetSomeHelp(char**);
void clear();
void changeDirectory(char*);
int getConsoleState();
void setConsoleState(int);
int executeMorePipe(char**, int&, int, char**);
