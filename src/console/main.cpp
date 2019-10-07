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

void printArguments(char **);

int main(int argc, char **argv, char **envp){
  char *inputBuffer;
  size_t bufsize = IN_BUFF;
  int sig;

  while(1) {
    inputBuffer = (char*) malloc(sizeof(char) * IN_BUFF);

    cout << "[Console: " << getWorkingDirectory() << "]$ " << std::flush;
    getline(&inputBuffer, &bufsize, stdin);
    queue<char**> commandQueue = setCommandQueue(inputBuffer);
    while (!commandQueue.empty()){
      char** args = commandQueue.front();
      commandQueue.pop();
      int execStatus = executeCommand(args, sig);
      if (execStatus == 0){
        if (sig == 1) {
          free(inputBuffer);
          return 0;
        }else if (sig == 2){
          system("clear");
        }else if (sig == 3){
          int success = chdir(args[1]);
          if (success == -1){
            cout << "Directory " << args[1] << " Does not exist in current path\n";
          }
        }else if (sig == 4){
          printEnviron();
        }else if (sig == 5){
          stopItGetSomeHelp();
        }
      }else{
        cout << "Unknown command or bad arguments in: " << inputBuffer << "\n" << std::flush;
      }
    }
    free(inputBuffer);
  }
  return 0;
}
