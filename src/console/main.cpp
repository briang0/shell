#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "util.h"
using namespace std;

#define IN_BUFF 256
#define PARAM_BUFF 15

void printArguments(char **);

int main(){
  char *inputBuffer = (char*) malloc(sizeof(char) * IN_BUFF * PARAM_BUFF);
  char **argumentBuffer = (char**) malloc(sizeof(char) * IN_BUFF * PARAM_BUFF);
  int status, wpid;
  size_t bufsize = IN_BUFF;

  for (int i = 0; i < 5; i++){
    cout << i << " [Console]: " << std::flush;
    getline(&inputBuffer, &bufsize, stdin);
    int pid = fork();

    if (pid == 0){
      setCommandBuffer(inputBuffer, argumentBuffer);
      int error = execvp(argumentBuffer[0], argumentBuffer);
      cout << error << "\n" << std::flush;
      exit(0);
    }else{
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
  }
  return 0;
}

void printArguments(char **args){
  int i = 0;
  cout << "Arguments:\n" << std::flush;
  while (i < PARAM_BUFF && args[i] != NULL){
    cout << args[i] << "\n" << std::flush;
    i++;
  }
}
