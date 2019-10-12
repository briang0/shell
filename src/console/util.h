//  Brian Guidarini
//  October 10th, 2019
//  Header file for the utility file. It just contains prototypes.
#include<queue>
char** getCommand(char*);
char* getWorkingDirectory();
std::queue<char**> setCommandQueue(char*);
char getLastNonWhitespaceCharacter(char*);
char** getRedirectionData(char**);
char** getArgsWithoutBackgroundOp(char**);
char** getArgsWithoutRedirectionOps(char**);
char* getLastItemInStrArr(char**);
char** getArgsWithoutPipeOp(char**);
