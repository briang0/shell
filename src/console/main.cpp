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
#define PARAM_BUFF 15

void printArguments(char **);

int main(int argc, char **argv, char **envp){
  char *inputBuffer;
  char **argumentBuffer;
  size_t bufsize = IN_BUFF;
  int sig;
  queue<char**> commandQueue;

  while(1) {
    inputBuffer = (char*) malloc(sizeof(char) * IN_BUFF * PARAM_BUFF);
    argumentBuffer = (char**) malloc(sizeof(char) * IN_BUFF * PARAM_BUFF);

    cout << "[Console: " << getWorkingDirectory() << "]$ " << std::flush;
    getline(&inputBuffer, &bufsize, stdin);
    setCommandBuffer(inputBuffer, argumentBuffer);
    int execStatus = executeCommand(argumentBuffer, sig);
    if (execStatus == 0){
      if (sig == 1) {
        break;
      }else if (sig == 2){
        system("clear");
      }else if (sig == 3){
        int success = chdir(argumentBuffer[1]);
        if (success == -1){
          cout << "Directory " << argumentBuffer[1] << " Does not exist in current path\n";
        }
      }else if (sig == 4){
        printEnviron();
      }else if (sig == 5){
        stopItGetSomeHelp();
      }
    }else{
      cout << "Unknown command or bad arguments in: " << inputBuffer << "\n" << std::flush;
    }
    free(inputBuffer);
    free(argumentBuffer);
  }
  return 0;
}

void printArguments(char **args){
  int i = 0;
  cout << "Arguments:\n" << std::flush;
  while (i < PARAM_BUFF){
    cout << args[i] << "\n" << std::flush;
    if (args[i] == NULL){
      cout << "NULL\n" << std::flush;
    }
    i++;
  }
}
