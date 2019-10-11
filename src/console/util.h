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
