#include<queue>
char** getCommand(char*);
char* getWorkingDirectory();
std::queue<char**> setCommandQueue(char*);
char getLastNonWhitespaceCharacter(char*);
char* getOutputFile(char**, int&);
