#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "util.h"
#include "console.h"
#include<string.h>
#include<queue>
using namespace std;

#define IN_BUFF 256

int main(int argc, char **argv, char **envp){
  char *inputBuffer;
  size_t bufsize = IN_BUFF;
  int sig;
  int paused = 0;
  int consoleState = 0;

  while(1) {
    inputBuffer = (char*) malloc(sizeof(char) * IN_BUFF);
    cout << "[Console:" << getWorkingDirectory() << "]$ " << std::flush;
    getline(&inputBuffer, &bufsize, stdin);
    char lastChar = getLastNonWhitespaceCharacter(inputBuffer);
    int bg = 0;
    if (lastChar == '&') bg = 1;
    queue<char**> commandQueue = setCommandQueue(inputBuffer);
    int a = 0;

    while (!commandQueue.empty()){
      char** args = commandQueue.front();
      commandQueue.pop();
      int execStatus = executeCommand(args, sig, bg);

      if (execStatus == 0){
        if (sig == 1) {
          free(inputBuffer);
          return 0;
        }else if (sig == 2){
          clear();
        }else if (sig == 3){
          changeDirectory(args[1]);
        }else if (sig == 4){
          printEnviron();
        }else if (sig == 5){
          stopItGetSomeHelp();
        }else if (sig == 6){
          setConsoleState(1);
        }
      }else{
        cout << "Unknown command or bad arguments in: " << inputBuffer << "\n" << std::flush;
      }
      consoleState = getConsoleState();
      if (consoleState == 1){
        cout << "Console is paused. Press enter to continue.\n";
        cin.ignore();
        setConsoleState(0);
      }

    }

    free(inputBuffer);
  }
  return 0;
}
