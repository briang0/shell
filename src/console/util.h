#include<queue>
char** getCommand(char*);
char* getWorkingDirectory();
std::queue<char**> setCommandQueue(char*);
char getLastNonWhitespaceCharacter(char*);
char** getRedirectionData(char**);
char** getArgsWithoutBackgroundOp(char** args);
char** getArgsWithoutRedirectionOps(char**);
