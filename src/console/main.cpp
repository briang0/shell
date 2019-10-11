#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "util.h"
#include "console.h"
#include "io.h"
#include <string.h>
#include <queue>
using namespace std;

#define IN_BUFF 512

queue<char**> getCommandQueue(int read, char* path, char* inputBuffer) {
  queue<char**> commandQueue;
  if (read == 1){
    commandQueue = parseCommandsIntoQueueFromFile(path);
  }else{
    commandQueue = setCommandQueue(inputBuffer);
  }
  return commandQueue;
}

//Main function that runs the console.
//argc = number of arguments
//argv = arguments
//envp = environmental vars
int main(int argc, char **argv, char **envp){
  char *inputBuffer;
  size_t bufsize = IN_BUFF;
  int sig;
  int paused = 0;
  int consoleState = 0;
  int read = 0;
  if (argc >= 2){
    read = 1;
  }
  while(1) {
    inputBuffer = (char*) malloc(sizeof(char) * IN_BUFF);
    cout << "[Console:" << getWorkingDirectory() << "]$ " << std::flush;
    if (!read)
      getline(&inputBuffer, &bufsize, stdin);
    char lastChar = getLastNonWhitespaceCharacter(inputBuffer);
    int bg = 0;
    if (lastChar == '&') bg = 1;
    queue<char**> commandQueue = getCommandQueue(read, argv[1], inputBuffer);
    read = 0;
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
